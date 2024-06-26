#pragma once
#include "../Quarzum.h"

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
    PARAM_CALL,
    AND,
    OR,
    REASSIGN,
    EQ,
    LOW,
    NEQ,
    GRT,
    GRTEQ,
    LOWEQ,
    NOT,
    INC
};

struct IRInstruction
{
    IRInstructionType type;
    std::string target;
    std::string origin1;
    std::string origin2;
    std::string varType;
};

const std::unordered_map<std::string, IRInstructionType> operatorToType = {
    {"+", ADD},
    {"-", SUB},
    {"*", PROD},
    {"and", AND},
    {"or", OR},
    {"==", EQ},
    {"<", LOW},
    {"<=", LOWEQ},
    {">", GRT},
    {">=", GRTEQ},
    {"!=", NEQ},
    {"not", NOT},
    {"++", INC}
};

IRInstructionType getInstructionType(const std::string& op){
    auto it = operatorToType.find(op);
    if(it != operatorToType.end()){
        return it->second;
    }
    return ADD;
}

u_int8_t cIndex;
u_int8_t lIndex;
u_int8_t tIndex;
u_int8_t vIndex;
inline const std::string getVIndex() noexcept{
    return ".v"+std::to_string(vIndex);
}
inline const std::string getTIndex() noexcept{
    return ".t"+std::to_string(tIndex);
}
inline const std::string getLIndex() noexcept{
    return ".l"+std::to_string(lIndex);
}
inline const std::string getCIndex() noexcept{
    return ".c"+std::to_string(cIndex);
}
std::vector<IRInstruction> ir;