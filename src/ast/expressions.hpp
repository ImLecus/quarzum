#pragma once
#include "astnode.hpp"

struct UnaryExpression : public Expression {
    std::string op; 
    Expression* operand;

    void check() override {
        if(op == "not"){
            this->type = operand->type->notg();
        }
    }

    UnaryExpression(const std::string& oper, Expression* expr) : op(oper), operand(expr) {}

    void print() {
        std::cout << "unary expression: (" << op << ")\n\t";
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

    void check() override {
        left->check();
        right->check();
        if(op == "+"){
            this->type = GenericType::sum(sortTypes(left->type, right->type));      
        }
        else if(op == "-"){
            this->type = left->type->sub(right->type);        
        }
        else if(op == "#"){
            this->type = GenericType::csum(sortTypes(left->type, right->type)); 
        }
        else if(op == "*"){
            this->type = GenericType::prod(sortTypes(left->type, right->type)); 
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
        else if(op == "and"){
            this->type = GenericType::andg(left->type, right->type); 
        }
        else if(op == "or"){
            this->type = GenericType::org(left->type, right->type); 
        }
        else if(op == "=="){
            this->type = GenericType::equal(left->type, right->type); 
        }
        else if(op == "!="){
            this->type = GenericType::nequal(left->type, right->type); 
        }
    }

    BinaryExpression(const std::string& op, Expression* left, Expression* right): op(op), left(left), right(right) {

    }

    void print() override{
        std::cout << "BinaryExpression: (" << op << ")\n";
        std::cout << '\t';
        left->print();
        std::cout << '\t';
        right->print();
    }

    void generateIR() override {
        index = getTIndex();
        left->generateIR();
        right->generateIR();
        tIndex++;
        ir.push_back(IRInstruction{getInstructionType(op),index,left->index,right->index,type->name});
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
    void check() override{}
};