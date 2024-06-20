#pragma once
#include "astnode.hpp"

struct UnaryExpression : public Expression {
    std::string op; 
    Expression* operand;

    UnaryExpression(const std::string& oper, Expression* expr) : op(oper), operand(expr) {
        if(op == "not"){
            this->type = operand->type->notg();
        }

    }

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

    BinaryExpression(const std::string& op, Expression* left, Expression* right): op(op), left(left), right(right) {
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
    }

    void print() override{
        std::cout << "BinaryExpression: (" << op << ")\n";
        std::cout << '\t';
        left->print();
        std::cout << '\t';
        right->print();
    }

    void generateIR() override {
        left->generateIR();
        right->generateIR();
        
        index = getTIndex();
        std::cout << op << " " << index << ", " << left->index << ", " << right->index << "\n";
        ir.push_back(IRInstruction{getInstructionType(op),index,left->index,right->index});
        //tIndex++;
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