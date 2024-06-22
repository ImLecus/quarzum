#pragma once
#define CONVERGE(t) return new t(converge(this->bits, type->bits))
#define PROMOTE(t) return new t(promote(this->bits, type->bits))
#define DOUBLE_PROMOTE(t) return new t(promote(this->bits, type->bits, true))
#include <cmath>

/**
 * @brief Throws a TypeError due to incompatibility between two types.
*/
nullptr_t throwOperatorError(const std::string& op, const std::string& a, const std::string& b);
nullptr_t throwUnaryOperatorError(const std::string& op, const std::string& a);
/**
 * @brief Returns the max number of bits between two types.
*/
u_int8_t converge(const u_int8_t& a, const u_int8_t& b);
/**
 * @brief Promotes a type into the next two power capacity in bits.
 * @param doublePromote Promotes two times in a row (example: int8 -> int32)
*/
u_int8_t promote(const u_int8_t& a, const u_int8_t& b,const bool& doublePromote = false);
/**
 * @brief Returns the number of bits detailed at the end of the name.
*/
u_int8_t getBits(const std::string& name);



enum TypeFlags : u_int8_t {
    FUNCTION,
    NULL_T,
    BOOL,
    UINT,
    INT,
    NUMBER,
    DECIMAL,
    CHAR,
    STRING
};
/**
 * @brief Represents a generic type.
 * It has a name and virtual methods that will be overrided in each type according to their rules.
*/
struct GenericType{
    std::string name;
    bool constant;
    TypeFlags flag;
    u_int8_t bits;
    inline bool isNumeric() const {
        return flag >= 2 and flag <= 4;
    }
    inline bool isText() const {
        return flag == CHAR or flag == STRING;
    }
    // Unary operators
    virtual GenericType* notg(){ return throwUnaryOperatorError("not",this->name); }
    virtual GenericType* inc(){ return throwUnaryOperatorError("++",this->name); }

    // Commutative operators
    static GenericType* sum(GenericType** types);
    static GenericType* csum(GenericType** types);
    static GenericType* prod(GenericType** types);
    static GenericType* andg(GenericType* a, GenericType* b);
    static GenericType* org(GenericType* a, GenericType* b);
    static GenericType* equal(GenericType* a, GenericType* b);
    static GenericType* nequal(GenericType* a, GenericType* b);
    static GenericType* lower(GenericType* a, GenericType* b);
    static GenericType* lowereq(GenericType* a, GenericType* b);
    static GenericType* greater(GenericType* a, GenericType* b);
    static GenericType* greatereq(GenericType* a, GenericType* b);


    //  Non-commutative operators
    //virtual GenericType* sum(GenericType* type){ return throwOperatorError("+",this->name,type->name); }
    virtual GenericType* sub(GenericType* type){ return throwOperatorError("-",this->name,type->name); }
    virtual GenericType* div(GenericType* type){ return throwOperatorError("/",this->name,type->name); }
    virtual GenericType* pow(GenericType* type){ return throwOperatorError("^",this->name,type->name); }

    virtual GenericType* mod(GenericType* type){ return throwOperatorError("%",this->name,type->name); }
    
};

struct NumericType : public GenericType {
    const long MIN_VALUE;
    const long MAX_VALUE;  
    NumericType(u_int8_t bits = 32, const long min = 0, const long max = 1) : MIN_VALUE(min), MAX_VALUE(max){
        this->bits = bits;
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
/**
 * @brief Represents a void type. Can't be used in operations.
*/
struct Function;

GenericType** sortTypes(GenericType* a, GenericType* b){
    GenericType** result = new GenericType*[2];
    if(a->flag > b->flag){
        result[0]=b;
        result[1]=a;
        return result;
    }
    result[0]=a;
    result[1]=b;
    return result;
}
