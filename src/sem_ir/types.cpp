#pragma once
#include "types.hpp"
#include <cmath>

struct Int;
struct Uint;
struct Bool;

struct PrimitiveType {

};

struct TextType : PrimitiveType {

};

struct NumericType : PrimitiveType {
    uint8_t bits;
    static long double MIN_VALUE;
    static long double MAX_VALUE;
    NumericType(uint8_t b): bits(b) {}
    

};

struct Int : NumericType {
    Int(uint8_t b): NumericType(b) {}
    Int operator+ (const Uint& u) const;
    Int operator+ (const Int& i) const {
        return Int(8);
    }
};

struct Uint : NumericType {
    Uint(uint8_t b): NumericType(b) {}

    Int operator + (const Int& i) const {
        return Int(8);
    }
    Uint operator + (const Uint u) const {
        return Uint(8);
    }
};

struct Bool : Uint {
    Bool(): Uint(1) {
        MIN_VALUE = 0;
        MAX_VALUE = 1;
    }

    Bool operator or (const Bool& b) const {return Bool();}
    Bool operator and (const Bool& b) const {return Bool();}
};


Int Int::operator+ (const Uint& u) const{
    return u+ *this;
}