#pragma once

#include "../tokenizer/tokenlist.hpp"
#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include "../ast/declarations.hpp"
#include "../ast/statements.hpp"
#include "../ast/containers.hpp"
#include "../error.hpp"
#include "identation.hpp"
#include <array>
#include <stack>

/**
 * @brief Returns a Literal that represents the NULL value depending on the type.
*/
Literal* getNullValue(GenericType*& type);

template<typename T, typename U>
bool instanceOf(const U& object);

class Parser {
public:
    Parser(const TokenList& tokens);
    const RootNode parse();
private:
    TokenList tokens;
    size_t i;
    std::string lastType;
    IdentationManager identation;
    bool exitedIf = false;

    inline Token get(const size_t& index);
    inline bool ask(const TokenType& type, const int8_t& distance = 0);
    void expect(const TokenType& t, const char* description);
    bool consume(const TokenType& type, const int8_t& distance = 0);

    FunctionCall* parseFunctionCall();
    Expression* parseExpression(TokenList expressionTokens = TokenList());
    void parseEnum();
    void parseImport();
    GenericType* parseInheritance();
    Identifier* getIdentifier(const bool& noScope = false);
    std::vector<Argument*> parseArguments();
    std::vector<Element*> parseEnumElements();
    Element* parseIdWithOptionalValue();
    std::vector<Expression*> parseAgumentsInCall();
    void parseSimpleStatement(Statement* node);

    GenericType* parseType();
    VariableDeclaration* parseVar();
    FunctionContainer* parseFunction();
    VariableRedeclaration* parseRedec();

};