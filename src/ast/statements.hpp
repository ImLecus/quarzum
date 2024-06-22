#pragma once
#include "astnode.hpp"

struct ReturnStatement: public Statement {
    Expression* expression;
    ReturnStatement(Expression* expr): expression(expr) {};
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
    void check() override {

    }
    void generateIR() override {
        expression ->generateIR();
        ir.push_back(IRInstruction{EXIT, expression->index});
    }
};
struct WhileStatement: public Statement {
    Expression* condition;
    WhileStatement(Expression* condition): condition(condition) {};
    void check() override {
        
    }
};
struct BreakStatement: public Statement {
    void check() override {
        
    }
};
struct ContinueStatement: public Statement {
    void check() override {
        
    }
};

struct EnumStatement : public Statement {
    Identifier* name;
    GenericType* extend;
    std::vector<Element*> children;
    EnumStatement(std::vector<Element*>& children,Identifier*& name, GenericType*& extend) :children(children), name(name),extend(extend)  {}
    void check() override {
        
    }
};

struct ImportStatement : public Statement {
    std::string path;
    std::vector<Identifier*> imports;
    ImportStatement(std::vector<Identifier*>& imports,const std::string& path): imports(imports), path(path) {}
    void check() override {
        
    }
};

struct FunctionCall : public Statement {
    Identifier* identifier;
    std::vector<Expression*> args;
    FunctionCall(Identifier*& identifier, std::vector<Expression*>& args): identifier(identifier), args(args) {

    }
    void check() override {
        if(symbolTable.find(identifier->value) == nullptr){
            throwError("Symbol " + identifier->value + " was not defined");
        }
    }
    void generateIR() override {
        for(Expression* arg : args){
            arg->generateIR();
            if(auto i = dynamic_cast<Identifier*>(arg)){
                ir.push_back(IRInstruction{PARAM_CALL, arg->index});
                tIndex = 0;
                continue;
            }
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
    Argument(GenericType*& type, Identifier*& id, Expression* defaultValue = nullptr): type(type), id(id), defaultValue(defaultValue) {}
};