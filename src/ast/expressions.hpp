#include "astnode.hpp"
class UnaryExpression : public ASTNode {
public:
    std::string op; 
    ASTNode* operand;

    UnaryExpression(const std::string& oper, ASTNode* expr) : op(oper), operand(expr) {}

    const void print() override {
        std::cout << "UnaryExpression: (" << op << ")\n";
        operand->print();
    }

    ~UnaryExpression() {
        delete operand;
    }
};

class BinaryExpression : public ASTNode {
public:
    std::string op;
    ASTNode* left; 
    ASTNode* right;

    BinaryExpression(const std::string& op, ASTNode* left, ASTNode* right): op(op), left(left), right(right) {}

    const void print() override{
        std::cout << "BinaryExpression: (" << op << ")\n";
        left->print();
        right->print();
    }

    ~BinaryExpression() {
        delete left;
        delete right;
    }
};