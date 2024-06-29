/*
 * Quarzum Compiler - types.cpp
 * Version 1.0, 29/06/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef TYPES_CPP
#define TYPES_CPP
#include "types.h"

long double getNumMinValue(uint8_t bits){
    switch (bits)
    {
    case 16:
        return std::numeric_limits<_Float16>::lowest();
    case 32:
        return std::numeric_limits<float>::lowest();
    case 64:
        return std::numeric_limits<double>::lowest();
    default:
        return 0;
    }
}
long double getNumMaxValue(uint8_t bits){
    switch (bits)
    {
    case 16:
        return std::numeric_limits<_Float16>::max();
    case 32:
        return std::numeric_limits<float>::max();
    case 64:
        return std::numeric_limits<double>::max();
    default:
        return 0;
    }
}

struct BaseType {
    bool isNumeric;
    bool isText;
};

struct NumericType : public BaseType {
    NumericType(uint8_t size, const long double min, const long double max): 
        size(size), 
        MIN_VALUE(min), 
        MAX_VALUE(max){
            isNumeric = true;
            isText = false;
        }
    uint8_t size;
    const long double MIN_VALUE;
    const long double MAX_VALUE;
};

struct Int : public NumericType {
    Int(u_int8_t size): NumericType(size, -std::pow(2,size - 1), std::pow(2,size - 1) -1 ) {};
};

struct Uint : public NumericType {
    Uint(u_int8_t size): NumericType(size, 0, std::pow(2,size) -1 ) {};
};

struct Num: public NumericType {
    Num(u_int8_t size): NumericType(size, getNumMinValue(size), getNumMaxValue(size) ) {};
};

struct Bool : public NumericType {
    Bool(): NumericType(1,0,1) {};
};

BaseType strToType(const std::string& str){
    std::unordered_map<std::string, BaseType> types = {
        {"bool", Bool()},
        {"uint8", Uint(8)},
        {"uint16", Uint(16)},
        {"uint32", Uint(32)},
        {"uint", Uint(32)},
        {"uint64", Uint(64)},
        {"int8", Int(8)},
        {"int16", Int(16)},
        {"int32", Int(32)},
        {"int", Int(32)},
        {"int64", Int(64)},
        {"num16", Num(16)},
        {"num32", Num(32)},
        {"num", Num(32)},
        {"num64", Num(64)}
        // {"char", Char()},
        // {"string", String()},
        // {"function", Function()}
    };
    auto it = types.find(str);
    if(it == nullptr){
        // err
        return BaseType();
    }
    return it->second;

}

template<typename T>
T operator+ (const Bool& b,const T& type){
    if(type.isNumeric){
        return type;
    }
}

#endif
