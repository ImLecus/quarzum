#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include "../ast/nodes.hpp"
#include "../ast/declarations.hpp"

class Semantics {
public:
    void check(RootNode root){
        for(ASTNode* node : root.nodes){
            if(instanceOf<VariableDeclaration>(node)){
                VariableDeclaration* declaration = dynamic_cast<VariableDeclaration*>(node);
                Type* type = dynamic_cast<Type*>(declaration->type);
                if(type->type == "int" and not instanceOf<IntegerLiteral>(declaration->expression)){
                    throwTypeError("Invalid assignment type");
                }
            }
        }
    }

    std::string rootToIR(RootNode root){
        std::string ir = "";
        for(ASTNode* node : root.nodes){
            if(instanceOf<VariableDeclaration>(node)){
                VariableDeclaration* declaration = dynamic_cast<VariableDeclaration*>(node);
                Identifier* id = dynamic_cast<Identifier*>(declaration->identifier);
                if(instanceOf<IntegerLiteral>(declaration->expression)){
                    IntegerLiteral* value = dynamic_cast<IntegerLiteral*>(declaration->expression);
                    ir += id->name + " = "+ value->value + '\n';
                }
            }
        }
        return ir;
    }
private:

    template<typename T, typename U>
    bool instanceOf(const U& object){
        if(T* type = dynamic_cast<T*>(object)){
            return true;
        }
        return false;
    }
};