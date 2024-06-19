#pragma once
#define CONVERGE(t) return new t(converge(this->bits, type->bits))
#define PROMOTE(t) return new t(promote(this->bits, type->bits))
#define DOUBLE_PROMOTE(t) return new t(promote(this->bits, type->bits, true))
#define GENERIC(op) GenericType* op(GenericType* type){ return GenericType::sum(type); }
#include <cmath>

/**
 * @brief Throws a TypeError due to incompatibility between two types.
*/
nullptr_t throwOperatorError(std::string op, std::string a, std::string b);
/**
 * @brief Returns the max number of bits between two types.
*/
u_int8_t converge(u_int8_t a, u_int8_t b);
/**
 * @brief Promotes a type into the next two power capacity in bits.
 * @param doublePromote Promotes two times in a row (example: int8 -> int32)
*/
u_int8_t promote(u_int8_t a, u_int8_t b, bool doublePromote = false);
/**
 * @brief Returns the number of bits detailed at the end of the name.
*/
u_int8_t getBits(std::string name);

/**
 * @brief Represents a generic type.
 * It has a name and virtual methods that will be overrided in each type according to their rules.
*/
struct GenericType{
    std::string name;
    bool constant;
    bool isNumeric = false;
    u_int8_t bits;
    virtual GenericType* sum(GenericType* type){ return throwOperatorError("+",this->name,type->name); }
    virtual GenericType* sub(GenericType* type){ return throwOperatorError("-",this->name,type->name); }
    virtual GenericType* prod(GenericType* type){ return throwOperatorError("*",this->name,type->name); }
    virtual GenericType* div(GenericType* type){ return throwOperatorError("/",this->name,type->name); }
    virtual GenericType* pow(GenericType* type){ return throwOperatorError("^",this->name,type->name); }
    virtual GenericType* csum(GenericType* type){ return throwOperatorError("#",this->name,type->name); }
    virtual GenericType* mod(GenericType* type){ return throwOperatorError("%",this->name,type->name); }
};

struct NumericType : public GenericType {
    long MIN_VALUE;
    long MAX_VALUE;  
    NumericType(u_int8_t bits = 32){
        this->bits = bits;
        isNumeric = true;
    }
};

struct DecimalType : public GenericType {
    long double MIN_VALUE;
    long double MAX_VALUE;  
    DecimalType(u_int8_t bits = 32){
        this->bits = bits;
    }
};
/**
 * @brief Represents a boolean type with values between 0 and 1.
*/
struct Boolean;
/**
 * @brief Represents an unsigned integer of 8/16/32/64 bits.
*/
struct UInteger;
/**
 * @brief Represents an signed integer of 8/16/32/64 bits.
*/
struct Integer;
/**
 * @brief Represents a character.
*/
struct Character;
/**
 * @brief Represents an array of characters.
*/
struct String;
/**
 * @brief Represents the null value.
*/
struct NullType;
/**
 * @brief Represents a floating point type of 32/64 bits.
*/
struct Number;
/**
 * @brief Represents a fixed point type of 64 bits.
*/
struct Decimal;
/**
 * @brief Represents a variable type.
*/
struct Var;