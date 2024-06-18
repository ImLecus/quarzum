#pragma once
#include <cmath>

struct GenericType{
};

struct NumericType : public GenericType {
    u_int8_t bits = 32;
    long MIN_VALUE;
    long MAX_VALUE;  
    NumericType(u_int8_t bits = 32): bits(bits){}
};

struct DecimalType : public GenericType {
    u_int8_t bits = 32;
    long double MIN_VALUE;
    long double MAX_VALUE;  
    DecimalType(u_int8_t bits = 32) : bits(bits) {}
};

struct UInteger : public NumericType {
    UInteger(u_int8_t bits = 32): NumericType(bits){
        MIN_VALUE = 0;
        MAX_VALUE = std::pow(2, bits) - 1;
    }

    UInteger operator + (UInteger* u){
        return UInteger(std::max(this->bits, u->bits));
    }
};

struct Integer : public NumericType {
    Integer(u_int8_t bits = 32): NumericType(bits){
        MIN_VALUE = -std::pow(2, bits - 1);
        MAX_VALUE = std::pow(2, bits - 1) - 1;
    }

    Integer operator + (Integer* i){
        return Integer(std::max(this->bits, i->bits));
    }

    Integer operator + (UInteger* u){
        return Integer(u->bits*2);
    }
};

struct Number : public DecimalType {
    Number(u_int8_t bits = 32): DecimalType(bits) {
        MIN_VALUE = bits == 32? __FLT32_MIN__ : __FLT64_MIN__;
        MAX_VALUE = bits == 32? __FLT32_MAX__ : __FLT64_MAX__;
    }
};

struct Decimal : public DecimalType {
    Decimal(): DecimalType(32) {
        MIN_VALUE = std::pow(10,-308);
        MAX_VALUE = std::pow(10,308);
    }
};