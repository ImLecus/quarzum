#pragma once
#include "../Quarzum.h"

enum IRInstructionType : uint8 {
    ASSIGN,
    ADD,
    SUB,
    PROD,
    DIV,
    MOD,
    CSUM,
    GOTO,
    LABEL,
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
    string target;
    string origin1;
    string origin2;
    string varType;
};

const std::unordered_map<string, IRInstructionType> operatorToType = {
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

IRInstructionType getInstructionType(const string& op){
    auto it = operatorToType.find(op);
    if(it != operatorToType.end()){
        return it->second;
    }
    return ADD;
}

uint8 cIndex;
uint8 lIndex;
uint8 tIndex;
uint8 vIndex;
inline const string getVIndex() noexcept{
    return ".v"+std::to_string(vIndex);
}
inline const string getTIndex() noexcept{
    return ".t"+std::to_string(tIndex);
}
inline const string getLIndex() noexcept{
    return ".l"+std::to_string(lIndex);
}
inline const string getCIndex() noexcept{
    return ".c"+std::to_string(cIndex);
}
std::vector<IRInstruction> ir;