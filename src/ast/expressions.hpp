#pragma once
#include "astnode.hpp"

struct UnaryExpression : public Expression {
    std::string op; 
    Expression* operand;

    UnaryExpression(const std::string& oper, Expression* expr) : op(oper), operand(expr) {

    }

    void print() override {
        
        std::cout << "UnaryExpression: (" << op << ")\n\t";
        operand->print();
    }

    ~UnaryExpression() {
        delete operand;
    }
};

struct BinaryExpression : public Expression {
    std::string op;
    Expression* left; 
    Expression* right;

    BinaryExpression(const std::string& op, Expression* left, Expression* right): op(op), left(left), right(right) {
        if(op == "+"){
            this->type = left->type->sum(right->type);      
        }
        else if(op == "-"){
            this->type = left->type->sub(right->type);        
        }
        else if(op == "#"){
            this->type = left->type->csum(right->type);
        }
        else if(op == "*"){
            this->type = left->type->prod(right->type);
        }
        else if(op == "/"){
            this->type = left->type->div(right->type);
        }
        else if(op == "%"){
            this->type = left->type->mod(right->type);
        }
        else if(op == "^"){
            this->type = left->type->pow(right->type);
        }
    }

    void print() override{
        std::cout << "BinaryExpression: (" << op << ")\n";
        std::cout << '\t';
        left->print();
        std::cout << '\t';
        right->print();
    }

    ~BinaryExpression() {
        delete left;
        delete right;
    }
};

struct NullExpression : public Expression {
    NullExpression(){}
    void print() override{
        std::cout << "NullExpression\n";
    }
};