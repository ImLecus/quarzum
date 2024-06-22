#pragma once
#include "../ir/irinstruction.hpp"
#include <string>
#include <memory>
#include <unordered_map>
#if defined(__x86_64__) || defined(__x86_64)
#define ARCHITECTURE "x86_64"
#endif

struct Assembler {
    std::vector<IRInstruction> instructions;
    Assembler(std::vector<IRInstruction> instructions): instructions(instructions) {} 
    virtual std::string assemble() = 0;
};

struct x86_64_Assembler : public Assembler {
    x86_64_Assembler(std::vector<IRInstruction> instructions): Assembler(instructions) {}
    std::unordered_map<std::string, std::string> typeToAsm = {
        {"int8", ".byte"},
        {"int16", ".word"},
        {"int32", ".long"},
        {"int64", ".quad"},
        {"bool", ".byte"},
        {"null", ".byte"},
        {"uint8", ".byte"},
        {"uint16", ".word"},
        {"uint32", ".long"},
        {"uint64", ".quad"},
        {"char", ".byte"},
        {"string", ".string"}
    };
    uint8_t paramRegister = 0;
    uint8_t temporalRegister = 0;
    std::string paramRegisters[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    std::string tempRegisters[6] = {"%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};
    std::string getParamRegister(){
        return paramRegisters[paramRegister];
    }
    std::string getTRegister(){
        return tempRegisters[temporalRegister];
    }
    std::string getValue(std::string i, std::string var = ""){
        if(i[0] == '.' and i[1] == 't'){
            temporalRegister = i[2]- '0';
            return tempRegisters[temporalRegister] + op_size(var);
        }
        if(isalpha(i[0])){
            return i;
        }
        return "$" + i;
    }  
    std::string create_operation(const std::string& opcode, 
    const std::string& value1 = "", const std::string& value2 = ""){
        return '\t'+opcode+' '+value1+(value2.empty()? "":", " + value2)+'\n';
    }
    std::string op_size(const std::string& var){
        if(var == ""){return var;}
        uint8_t bits = getBits(var);
        if(bits == 64){return "q";}
        if(bits == 32){return "d";}
        if(bits == 16){return "w";}
        if(bits == 8 or var == "bool"){return "b";}
        return "d";
    }
    std::string assemble(){
        std::string dataSection = ".data\n";
        std::string textSection = ".text\n.globl _start\n";
        for(auto i : instructions){
            switch (i.type)
            {
            case PARAM_CALL:
                textSection += "\tmovq $" + i.target + ", " + getParamRegister() + '\n';
                paramRegister++;
                break;
            case CALL:
                textSection += create_operation("call", i.target);
                paramRegister = 0;
                break;
            case LABEL:
                if(i.target == "main"){textSection += "_start:\n";break;}
                textSection += i.target + ":\n";
                break;
            case ASSIGN:
                dataSection += "\t" + i.target + ": "+ typeToAsm.at(i.varType); 
                if(i.origin2 == "literal"){
                    dataSection += " " + i.origin1 + "\n";
                    break;
                }
                dataSection += " 0\n";
                textSection += create_operation("mov",getValue(i.origin1,i.varType),i.target);
                break;
            case REASSIGN:
                if(i.origin2 == "literal"){
                    textSection += "\tmovb $" + i.origin1 + ", " + i.target + "\n"; 
                    break;
                }
                textSection += create_operation("mov",getTRegister(),i.target);
                break;
            case GOTO:
                if(not i.origin1.empty()){
                    textSection += "\tcmpb $1, " + (i.origin2=="literal"? i.origin1 : getValue(i.origin1)+"b") + "\n";
                    textSection += create_operation("je", i.target);
                    break;
                }
                textSection += create_operation("jmp", i.target);
                break;
            case EQ:
                if(i.origin2 == i.target){
                    i.target = getValue(i.target,i.varType);
                    textSection += create_operation("cmp",getValue(i.origin1,i.varType),i.target);
                    textSection += create_operation("setz", i.target);
                    break;
                }
                if(i.origin1 == i.target){
                    i.target = getValue(i.target,i.varType);
                    textSection +=  create_operation("cmp") + getValue(i.origin2,i.varType) + ", " + i.target  + "\n";
                    textSection += create_operation("setz", i.target);
                    break;
                }
                i.target = getValue(i.target,i.varType);
                textSection += create_operation("mov",getValue(i.origin1,i.varType),i.target);
                textSection += create_operation("cmp",getValue(i.origin2,i.varType),i.target);
                textSection += create_operation("setz", i.target);
                break;
            case NEQ:
                if(i.origin2 == i.target){
                    i.target = getValue(i.target,i.varType);
                    textSection += create_operation("cmp",getValue(i.origin1,i.varType),i.target);
                    textSection += create_operation("setnz", i.target);
                    break;
                }
                if(i.origin1 == i.target){
                    i.target = getValue(i.target,i.varType);
                    textSection +=  create_operation("cmp") + getValue(i.origin2,i.varType) + ", " + i.target  + "\n";
                    textSection += create_operation("setnz", i.target);
                    break;
                }
                i.target = getValue(i.target,i.varType);
                textSection += create_operation("mov",getValue(i.origin1,i.varType),i.target);
                textSection += create_operation("cmp",getValue(i.origin2,i.varType),i.target);
                textSection += create_operation("setnz", i.target);
                break;
            case AND:
                if(i.origin2 == i.target){
                    textSection += "\tandb " + i.origin1 + ", " + getTRegister() + "b\n";
                    break;
                }
                if(i.origin1 == i.target){
                    textSection += "\tandb " + i.origin2 + ", " + getTRegister() + "b\n";
                    break;
                }
                if(i.origin1[1]=='t' or i.origin2[1]=='t'){temporalRegister++;}
                textSection += "\tmovb " + i.origin1 + ", " + getTRegister() + "b\n";
                textSection += "\tandb " + i.origin2 + ", " + getTRegister() + "b\n";
                break;
            case OR:
                if(i.origin2 == i.target){
                    textSection += "\torb " + i.origin1 + ", " + getTRegister() + "b\n";
                    break;
                }
                if(i.origin1 == i.target){
                    textSection += "\torb " + i.origin2 + ", " + getTRegister() + "b\n";
                    break;
                }
                if(i.origin1[1]=='t' or i.origin2[1]=='t'){temporalRegister++;}
                textSection += "\tmovb " + i.origin1 + ", " + getTRegister() + "b\n";
                textSection += "\torb " + i.origin2 + ", " + getTRegister() + "b\n";
                break;
            case ADD:
                if(i.origin2 == i.target){
                    i.target = getValue(i.target,i.varType);
                    textSection += create_operation("add", getValue(i.origin1,i.varType),i.target);
                    break;
                }
                if(i.origin1 == i.target){
                    i.target = getValue(i.target,i.varType);
                    textSection += create_operation("add", getValue(i.origin2, i.varType),i.target);
                    break;
                }
                i.target = getValue(i.target,i.varType);
                textSection += create_operation("mov",getValue(i.origin1, i.varType),i.target );
                textSection += create_operation("add",getValue(i.origin2, i.varType),i.target);
                break;
            case SUB: 
                if(i.origin2 == i.target){
                    i.target = getValue(i.target);
                    textSection += create_operation("sub",getValue(i.origin1), i.target);
                    break;
                }
                if(i.origin1 == i.target){
                    i.target = getValue(i.target);
                    textSection += create_operation("sub",getValue(i.origin2), i.target);
                    break;
                }
                i.target = getValue(i.target);
                textSection += create_operation("mov",getValue(i.origin1),i.target);
                textSection += create_operation("cmp",getValue(i.origin2),i.target);
                break;
            case EXIT:
                textSection += "\tmovq $60, %rax\n\tmovq " + getValue(i.target) + ", %rdi\n\tsyscall\n";
                break;
            default:
                break;
            }
        }
        return dataSection + textSection;
    }
};

std::unique_ptr<Assembler> getAssembler(std::vector<IRInstruction> instructions,const char* mode = ARCHITECTURE){
    if(mode == "x86_64"){
        return std::make_unique<x86_64_Assembler>(instructions);
    }
    return nullptr;
}