#pragma once
#include "types.hpp"

nullptr_t throwOperatorError(const std::string op, const std::string a, const std::string b){
    throwTypeError("Operation '" + op + "' does not exist between types " + a + " and " + b);
    return nullptr;
}
nullptr_t throwUnaryOperatorError(const std::string op, const std::string a){
    throwTypeError("Operation '" + op + "' does not exist for type " + a );
    return nullptr;
}
u_int8_t converge(const u_int8_t a, const u_int8_t b){
    return std::max(a,b);
}

u_int8_t promote(const u_int8_t a, const u_int8_t b, bool doublePromote){
    return converge(a,b) * (a>=32 or b>=32? 1: 2 * (1+doublePromote));
}

u_int8_t getBits(const std::string name){
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
        flag = BOOL;
    }
    GenericType* notg(){
        return new Boolean();
    }
};

struct UInteger : public NumericType {
    UInteger(u_int8_t bits = 32): NumericType(bits, 0, std::pow(2,bits)-1){
        name = "uint" + std::to_string(bits);
        flag = UINT;
    }
};

struct Integer : public NumericType {
    Integer(u_int8_t bits = 32): NumericType(bits,-std::pow(2, bits - 1), std::pow(2, bits - 1) - 1){
        name = "int" + std::to_string(bits);
        flag = INT;
    }

    GenericType* sub(GenericType* type) override {
        if(type->isNumeric()){
            return new Integer(promote(this->bits, type->bits));
        }
        return GenericType::sub(type);
    }

    Integer* div(NumericType* type){PROMOTE(Integer);}
    Integer* mod(NumericType* type){CONVERGE(Integer);}   
    // TO-DO: change rules
    Integer* pow(NumericType* type){DOUBLE_PROMOTE(Integer);}

};

struct Number : public DecimalType {
    Number(u_int8_t bits = 32): DecimalType(bits) {
        MIN_VALUE = bits == 32? __FLT32_MIN__ : __FLT64_MIN__;
        MAX_VALUE = bits == 32? __FLT32_MAX__ : __FLT64_MAX__;
        flag = NUMBER;
    }
};

struct Decimal : public DecimalType {
    Decimal(): DecimalType(32) {
        MIN_VALUE = std::pow(10,-308);
        MAX_VALUE = std::pow(10,308);
        flag = DECIMAL;
    }
};

struct Character : public GenericType {
    Character(){
        name = "char";
        flag = CHAR;
    }

};

struct String : public GenericType {
    String(){
        name = "string";
        flag = STRING;
    }
};

struct NullType : public GenericType
{
    u_int8_t bits = 0;
    NullType(){name = "null"; flag = NULL_T;}
};

struct Var : public GenericType {
    Var(){
        name = "var";
    }
};

struct Function : public GenericType {
    Function(){
        name = "function";
        flag = FUNCTION;
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
    if(name == "function"){
        return new Function();
    }
    return new Var();
}

GenericType* GenericType::sum(GenericType** types){ 
    if(types[0]->flag == NULL_T){return types[1];}
    if(types[0]->flag == INT and types[1]->isNumeric()){
        return new Integer(promote(types[0]->bits,types[1]->bits));
    }
    if(types[0]->flag == UINT and types[1]->isNumeric()){
        return new UInteger(promote(types[0]->bits,types[1]->bits));
    }
    return throwOperatorError("+",types[0]->name,types[1]->name); 
}
GenericType* GenericType::prod(GenericType** types){ 
    if(types[0]->flag == INT and types[1]->flag == STRING){
        return new String();
    }
    if(types[0]->isNumeric() and types[1]->isNumeric()){
        return new Integer(promote(types[0]->bits, types[1]->bits));
    }
    return throwOperatorError("*",types[0]->name,types[1]->name); 
}
GenericType* GenericType::csum(GenericType** types){ 
    switch (types[0]->flag)
    {
    case NULL_T:
        return types[1];
    case BOOL:
        if(types[1]->isNumeric()){
            return types[1];
        }
        return throwOperatorError("#",types[0]->name,types[1]->name); 
    default:
        return throwOperatorError("#",types[0]->name,types[1]->name); 
    }    
}
GenericType* GenericType::andg(GenericType* a, GenericType* b){ 
    if(a->flag == BOOL and b->flag == BOOL){
        return new Boolean();
    }
    return throwOperatorError("and",a->name,b->name); 
}
GenericType* GenericType::org(GenericType* a, GenericType* b){ 
    if(a->flag == BOOL and b->flag == BOOL){
        return new Boolean();
    }
    return throwOperatorError("or",a->name,b->name); 
}
GenericType* GenericType::equal(GenericType* a, GenericType* b){ 
    if(a->flag == b->flag){
        return new Boolean();
    }
    return throwOperatorError("==",a->name,b->name); 
}
