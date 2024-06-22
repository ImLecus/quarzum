#pragma once
#include "../ir/irinstruction.hpp"
#include <string>
#include <memory>
#include <unordered_map>
#if defined(__x86_64__) || defined(__x86_64)
#define ARCHITECTURE "x86_64"
#endif

struct Assembler {
    const std::vector<IRInstruction> instructions;
    Assembler(std::vector<IRInstruction> instructions): instructions(instructions) {} 
    virtual std::string assemble() = 0;
};

struct x86_64_Assembler : public Assembler {
    x86_64_Assembler(std::vector<IRInstruction> instructions): Assembler(instructions) {}
    const std::unordered_map<std::string, const char*> typeToAsm = {
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
    const char* paramRegisters[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    const char* tempRegisters[6] = {"%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};
    inline const char* getParamRegister() const {
        return paramRegisters[paramRegister];
    }
    inline const char* getTRegister() const{
        return tempRegisters[temporalRegister];
    }
    const std::string getValue(const std::string& i,const std::string& var = ""){
        if(i[0] == '.' and i[1] == 't'){
            temporalRegister = i[2]- '0';
            return tempRegisters[temporalRegister] + op_size(var);
        }
        if(isalpha(i[0])){
            return i;
        }
        return "$" + i;
    }  
    inline const std::string create_operation(const std::string& opcode, const std::string& value1 = "", const std::string& value2 = ""){
        return '\t'+opcode+' '+value1+(value2.empty()? "":", " + value2)+'\n';
    }
    const std::string op_size(const std::string& var){
        if(var == ""){return var;}
        uint8_t bits = getBits(var);
        if(bits == 64){return "q";}
        if(bits == 32){return "d";}
        if(bits == 16){return "w";}
        if(bits == 8 or var == "bool"){return "b";}
        return "d";
    }
    void create_logical_operation(std::string& textSection,IRInstruction& i, const std::string& op){
        if(i.origin2 == i.target){
            i.target = getValue(i.target,i.varType);
            textSection += create_operation("cmp",getValue(i.origin1,i.varType),i.target);
            textSection += create_operation(op, i.target);
            return;
        }
        if(i.origin1 == i.target){
            i.target = getValue(i.target,i.varType);
            textSection +=  create_operation("cmp") + getValue(i.origin2,i.varType) + ", " + i.target  + "\n";
            textSection += create_operation(op, i.target);
            return;
        }
        i.target = getValue(i.target,i.varType);
        textSection += create_operation("mov",getValue(i.origin1,i.varType),i.target);
        textSection += create_operation("cmp",getValue(i.origin2,i.varType),i.target);
        textSection += create_operation(op, i.target);
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
                
                textSection += create_operation("mov",getValue(i.origin1, i.varType),i.target);
                break;
            case GOTO:
                if(not i.origin1.empty()){
                    textSection += "\tcmpb $1, " + (i.origin2=="literal"? i.origin1 : getValue(i.origin1)+"b") + "\n";
                    textSection += create_operation("je", i.target);
                    break;
                }
                textSection += create_operation("jmp", i.target);
                break;
            case INC:
                textSection += create_operation("inc",i.target);
                break;
            case EQ:
                create_logical_operation(textSection, i,"setz");
                break;
            case NEQ:
                create_logical_operation(textSection, i, "setnz");
                break;
            case LOW:
                create_logical_operation(textSection, i, "setl");
                break;
            case LOWEQ:
                create_logical_operation(textSection, i, "setle");
                break;
            case GRT:
                create_logical_operation(textSection, i, "setg");
                break;
            case GRTEQ:
                create_logical_operation(textSection, i, "setge");
                break;
            case NOT:
                textSection += create_operation("mov", i.target, getTRegister());
                textSection += create_operation("not",getTRegister());

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

Assembler* getAssembler(const std::vector<IRInstruction>& instructions,const char* mode = ARCHITECTURE){
    if(mode == "x86_64"){
        return new x86_64_Assembler(instructions);
    }
    return nullptr;
}