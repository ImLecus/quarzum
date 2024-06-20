#pragma once
#include <vector>
#include <stack>

enum IRInstructionType : u_int8_t {
    ASSIGN,
    ADD,
    SUB,
    PROD,
    DIV,
    MOD,
    CSUM,
    IF,
    GOTO,
    LABEL
};

struct IRInstruction
{
    IRInstructionType type;
    std::string target;
    std::string origin1;
    std::string origin2;
};

IRInstructionType getInstructionType(std::string op){
    if(op == "+"){return ADD;}
    if(op == "-"){return SUB;}
    if(op == "*"){return PROD;}
    return ADD;
}

u_int16_t tIndex;
std::string getTIndex(){
    return "t"+std::to_string(tIndex);
}
std::vector<IRInstruction> ir;