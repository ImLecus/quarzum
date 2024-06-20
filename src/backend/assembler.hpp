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
    uint8_t paramRegister;
    std::string registers[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "r8", "r9"};
    std::string getParamRegister(){
        
        return registers[paramRegister];
    }
    std::string assemble(){
        std::string dataSection = ".data\n";
        std::string textSection = ".text\n.globl _start\n";
        for(auto i : instructions){
            switch (i.type)
            {
            case PARAM_CALL:
                textSection += "\tmov $" + i.target + ", " + getParamRegister() + '\n';
                break;
            case CALL:
                textSection += "\tcall " + i.target + '\n';
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
                break;
            case ADD:
                //textSection += "\tmov $" + i.origin1 + ", %r8\n";
                break;
            case EXIT:
                textSection += "\tmovb $60, %al\n\txorq %rdi, %rdi\n\tsyscall\n";
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