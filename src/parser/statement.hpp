#pragma once
#include <string>
#include "expression.hpp"
class Statement {
    public:
        Statement(){}

        std::string toString(){
           return "Statement";  
        }
    private:
};

class VarDeclarationStatement : public Statement {
    public:
        VarDeclarationStatement(std::string name, std::string type, Expression value) : name(name), type(type), value(value){}

        std::string toString(){
           return "VarDeclStatement: " + this->name + ": " + this->type;  
        }

    private:
        std::string name;
        std::string type;
        Expression value; // expr
};

class ReassignStatement : public Statement {
    public:
        ReassignStatement(std::string name, std::string type, Expression value) : name(name), value(value){}
        std::string toString(){
           return "RassignStatement: " + this->name;  
        }
    private:
        std::string name;
        Expression value; 
};