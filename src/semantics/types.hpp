#pragma once
#include <cmath>

#define CONVERGE(t) return new t(converge(this->bits, type->bits))
#define PROMOTE(t) return new t(promote(this->bits, type->bits))
#define DOUBLE_PROMOTE(t) return new t(promote(this->bits, type->bits, true))

nullptr_t throwOperatorError(std::string op, std::string a, std::string b){
    throwTypeError("Operation " + op + " does not exist between types " + a + " and " + b);
    return nullptr;
}
u_int8_t converge(u_int8_t a, u_int8_t b){
    return std::max(a,b);
}
u_int8_t promote(u_int8_t a, u_int8_t b, bool doublePromote = false){
    return converge(a,b) * (a>=32 or b>=32? 1: 2 * (1+doublePromote));
}

struct GenericType{
    std::string name;
    virtual GenericType* sum(GenericType* type){ return throwOperatorError("+",this->name,type->name); }
    virtual GenericType* sub(GenericType* type){ return throwOperatorError("-",this->name,type->name); }
    virtual GenericType* prod(GenericType* type){ return throwOperatorError("*",this->name,type->name); }
    virtual GenericType* div(GenericType* type){ return throwOperatorError("/",this->name,type->name); }
    virtual GenericType* pow(GenericType* type){ return throwOperatorError("^",this->name,type->name); }
    virtual GenericType* csum(GenericType* type){ return throwOperatorError("#",this->name,type->name); }
    virtual GenericType* mod(GenericType* type){ return throwOperatorError("%",this->name,type->name); }
    char id;
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
        name = "uint" + std::to_string(bits);
        MIN_VALUE = 0;
        MAX_VALUE = std::pow(2, bits) - 1;
    }

};

struct Integer : public NumericType {
    Integer(u_int8_t bits = 32): NumericType(bits){
        name = "int" + std::to_string(bits);
        MIN_VALUE = -std::pow(2, bits - 1);
        MAX_VALUE = std::pow(2, bits - 1) - 1;
    }

    GenericType* sum(GenericType* type){ return GenericType::sum(type); }
    Integer* sum(NumericType* type){PROMOTE(Integer);}   

    GenericType* sub(GenericType* type){return GenericType::sub(type); }
    Integer* sub(NumericType* type){PROMOTE(Integer);}
    
    GenericType* csum(GenericType* type){return GenericType::csum(type);}
    Integer* csum(NumericType* type){CONVERGE(Integer);}

    GenericType* prod(GenericType* type){return GenericType::prod(type);}
    Integer* prod(NumericType* type){PROMOTE(Integer);}

    GenericType* div(GenericType* type){return GenericType::div(type);}
    Integer* div(NumericType* type){PROMOTE(Integer);}

    GenericType* mod(GenericType* type) {return GenericType::mod(type);}
    Integer* mod(NumericType* type){CONVERGE(Integer);}   

    // TO-DO: change rules
    GenericType* pow(GenericType* type) {return GenericType::pow(type);}
    Integer* pow(NumericType* type){DOUBLE_PROMOTE(Integer);}

};

struct Number : public DecimalType {
    char id = 'n';
    Number(u_int8_t bits = 32): DecimalType(bits) {
        MIN_VALUE = bits == 32? __FLT32_MIN__ : __FLT64_MIN__;
        MAX_VALUE = bits == 32? __FLT32_MAX__ : __FLT64_MAX__;
    }
};

struct Decimal : public DecimalType {
    char id = 'd';
    Decimal(): DecimalType(32) {
        MIN_VALUE = std::pow(10,-308);
        MAX_VALUE = std::pow(10,308);
    }
};

struct Character : public GenericType {
    Character(){
        name = "char";
    }

};

struct String : public GenericType {
    String(){
        name = "string";
    }

    GenericType* sum(GenericType* type){ return GenericType::sum(type); }
    String* sum(Character* type){return new String();}   
    String* sum(String* type){return new String();}  

    GenericType* sub(GenericType* type){return GenericType::sub(type); }
    String* sub(Character* type){return new String();}
    String* sub(String* type){return new String();}  
    
    GenericType* csum(GenericType* type){return GenericType::csum(type);}

    GenericType* prod(GenericType* type){return GenericType::prod(type);}
    String* prod(UInteger* type){return new String();}

    GenericType* div(GenericType* type){return GenericType::div(type);}

    GenericType* mod(GenericType* type) {return GenericType::mod(type);}

    GenericType* pow(GenericType* type) {return GenericType::pow(type);}
};

struct NullType : public GenericType
{
    u_int8_t bits = 0;
    NullType(){
        name = "null";
    }

    GenericType* sum(GenericType* type){ return type; }   

    GenericType* sub(GenericType* type){return type; }
    
    GenericType* csum(GenericType* type){return type;}

    GenericType* prod(GenericType* type){return new NullType();}

    GenericType* div(GenericType* type){return GenericType::div(type);}

    GenericType* mod(GenericType* type) {return GenericType::mod(type);} 

    GenericType* pow(GenericType* type) {return GenericType::pow(type);}

};