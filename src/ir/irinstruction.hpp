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
    GOTO,
    LABEL,
    EXIT,
    PARAM,
    RET,
    CALL,
    PARAM_CALL
};

struct IRInstruction
{
    IRInstructionType type;
    std::string target;
    std::string origin1;
    std::string origin2;
    std::string varType;
};

IRInstructionType getInstructionType(std::string op){
    if(op == "+"){return ADD;}
    if(op == "-"){return SUB;}
    if(op == "*"){return PROD;}
    return ADD;
}

u_int8_t cIndex;
u_int8_t lIndex;
u_int8_t tIndex;
u_int8_t vIndex;
std::string getVIndex(){
    return "v"+std::to_string(vIndex);
}
std::string getTIndex(){
    return "t"+std::to_string(tIndex);
}
std::string getLIndex(){
    return "l"+std::to_string(lIndex);
}
std::string getCIndex(){
    return "c"+std::to_string(cIndex);
}
std::vector<IRInstruction> ir;