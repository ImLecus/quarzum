#include "astnode.hpp"

class VariableDeclaration : public ASTNode {
public:
    ASTNode* type;
    ASTNode* identifier;
    ASTNode* expression;

    VariableDeclaration(ASTNode* varType, ASTNode* varName, ASTNode* initValue): type(varType), identifier(varName), expression(initValue) {}

    const void print() override  {
        std::cout << "VariableDeclaration:\n";
        std::cout <<'\t';
        type->print();
        std::cout <<'\t';
        identifier->print();
        if (expression) {
            std::cout <<'\t';
            expression->print();
        }
    }

    ~VariableDeclaration() {
        delete type;
        delete identifier;
        delete expression;
    }
};