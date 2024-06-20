#pragma once
#include "astnode.hpp"

struct ReturnStatement: public Statement {
    Expression* expression;
    ReturnStatement(Expression* expr): expression(expr) {};
    void print() override {
        std::cout << "ret ";
        expression->print();
    }
    void check() override {
        
    }
    void generateIR() override {
        expression->generateIR();
        std::cout << "RET " << expression->index << '\n';
        ir.push_back(IRInstruction{RET, expression->index});
    }
};
struct ExitStatement: public Statement {
    Expression* expression;
    ExitStatement(Expression* expr): expression(expr) {};
    void print() override {
        std::cout << "exit ";
        expression->print();
    }
    void check() override {
    //   if(not instanceOf<UInteger>(expression->type)){
    //     throwTypeError("Exit escape code should be an uint");
    //   }

    }
    void generateIR() override {
        expression ->generateIR();
        ir.push_back(IRInstruction{EXIT, expression->index});
    }
};
struct WhileStatement: public Statement {
    Expression* condition;
    WhileStatement(Expression* condition): condition(condition) {};
    void print() override {
        std::cout << "while ";
        condition->print();
    }
    void check() override {
        
    }
};
struct BreakStatement: public Statement {
    void print() override {
        std::cout << "break\n";
    }
    void check() override {
        
    }
};
struct ContinueStatement: public Statement {
    void print() override {
        std::cout << "continue\n";
    }
    void check() override {
        
    }
};

struct EnumStatement : public Statement {
    Identifier* name;
    GenericType* extend;
    std::vector<Element*> children;
    EnumStatement(std::vector<Element*> children,Identifier* name, GenericType* extend) :children(children), name(name),extend(extend)  {}
    void print () override {
        std::cout << "enum:\n\t";
        name->print();
        std::cout << "type: " << extend->name << "\n\t";
        for(auto& element : children){
            element->print();
        }
    }
    void check() override {
        
    }
};

struct ImportStatement : public Statement {
    std::string path;
    std::vector<Identifier*> imports;
    ImportStatement(std::vector<Identifier*> imports, std::string path): imports(imports), path(path) {}
    void print() override {
        std::cout << "import: \n\t";
        std::cout << "path: " << this->path << "\n\t";
        for(auto& import : imports){
            import ->print();
        }
    }
    void check() override {
        
    }
};

struct FunctionCall : public Statement {
    Identifier* identifier;
    std::vector<Expression*> args;
    FunctionCall(Identifier* identifier, std::vector<Expression*> args): identifier(identifier), args(args) {}
    void print() override{
        std::cout << "FunctionCall:\n";
        identifier->print();
        for(auto& arg: args){
            arg->print();
        }
    }
    void check() override {
        
    }
    void generateIR() override {
        for(Expression* arg : args){
            arg->generateIR();
            if(auto l = dynamic_cast<Literal*>(arg)){
                ir.push_back(IRInstruction{ASSIGN, getVIndex(), arg->index,"literal",arg->type->name});
                ir.push_back(IRInstruction{PARAM_CALL, getVIndex()});
                vIndex++;
                tIndex = 0;
                continue;
            }
            ir.push_back(IRInstruction{PARAM_CALL, arg->index});
            tIndex = 0;
        }
        ir.push_back(IRInstruction{CALL, identifier->value});
    }
};

struct Argument : public ASTNode {
    GenericType* type;
    Identifier* id;

    Expression* defaultValue;
    Argument(GenericType* type, Identifier* id, Expression* defaultValue = nullptr): type(type), id(id), defaultValue(defaultValue) {}
    void print() override{
        std::cout << "Argument:\n\t"<<type->name<<"\n\t";
        id->print();
        if(defaultValue){
            defaultValue -> print();
        }
    }
};