#pragma once
#include "types.hpp"

nullptr_t throwOperatorError(std::string op, std::string a, std::string b){
    throwTypeError("Operation " + op + " does not exist between types " + a + " and " + b);
    return nullptr;
}
u_int8_t converge(u_int8_t a, u_int8_t b){
    return std::max(a,b);
}
u_int8_t promote(u_int8_t a, u_int8_t b, bool doublePromote){
    return converge(a,b) * (a>=32 or b>=32? 1: 2 * (1+doublePromote));
}
u_int8_t getBits(std::string name){
    u_int8_t result;
    if(name == "int" or name == "uint" or name == "number"){
        return 32;
    }
    if(isdigit(name[name.length() - 1])){
        
        result += (name[name.length() -1])- '0';
    }
    if(isdigit(name[name.length() - 2])){
        result += 10* (name[name.length() -2]- '0');
    }
    return result;
}


struct Boolean : public NumericType {
    Boolean(): NumericType(1){
        name = "bool";
        MIN_VALUE = 0;
        MAX_VALUE = 1;
    }
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
    DEFAULT_RULES

    Integer* sum(NumericType* type){PROMOTE(Integer);}   
    Integer* sub(NumericType* type){PROMOTE(Integer);}
    Integer* csum(NumericType* type){CONVERGE(Integer);}
    Integer* prod(NumericType* type){PROMOTE(Integer);}
    Integer* div(NumericType* type){PROMOTE(Integer);}
    Integer* mod(NumericType* type){CONVERGE(Integer);}   
    // TO-DO: change rules
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
    GENERIC(sum);

};

struct String : public GenericType {
    String(){
        name = "string";
    }
    DEFAULT_RULES

    String* sum(Character* type){return new String();}   
    String* sum(String* type){return new String();}  
    String* sub(Character* type){return new String();}
    String* sub(String* type){return new String();}  
    String* prod(UInteger* type){return new String();}
};

struct NullType : public GenericType
{
    u_int8_t bits = 0;
    NullType(){name = "null";}
    GenericType* sum(GenericType* type){ return type; }   
    GenericType* sub(GenericType* type){return type; }
    GenericType* csum(GenericType* type){return type;}
    GenericType* prod(GenericType* type){return new NullType();}
    GenericType* div(GenericType* type){return GenericType::div(type);}
    GenericType* mod(GenericType* type) {return GenericType::mod(type);} 
    GenericType* pow(GenericType* type) {return GenericType::pow(type);}
};

struct Var : public GenericType {
    Var(){
        name = "var";
    }
};

GenericType* getTypeByName(std::string name, bool constant = false){
    if(name.find("uint") != std::string::npos){
        return new UInteger(getBits(name));
    }
    if(name.find("int") != std::string::npos){
        return new Integer(getBits(name));
    }
    if(name == "string"){
        return new String();
    }
    if(name == "char"){
        return new Character();
    }
    if(name == "var"){
        return new Var();
    }
    if(name == "bool"){
        return new Boolean();
    }
    if(name == "number" or name == "number64"){
        return new Number(getBits(name));
    }
    if(name == "decimal"){
        return new Decimal();
    }
    return new Var();
}