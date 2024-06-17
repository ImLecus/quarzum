#pragma once

#include "../tokenizer/tokenlist.hpp"
#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include "../ast/nodes.hpp"
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
Literal* getNullValue(Type* type);

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

    Token get(const size_t& index);
    bool ask(const TokenType type,const int8_t distance = 0);
    void expect(const TokenType t, const char* description);

    TokenList getExpressionTerms();
    Expression* parseExpression(TokenList expressionTokens = TokenList());
    void parseEnum();
    void parseImport();
    void parseIfStatement();
    void parseWhileStatement();
    void parseModuleStatement();
    std::vector<ASTNode*> parseArguments();
    std::vector<ASTNode*> parseEnumElements();
    std::array<ASTNode*, 2> parseIdWithOptionalValue();
    std::vector<ASTNode*> parseAgumentsInCall();
    void parseSimpleStatement(Statement* node);
    Symbol* parseTypeAndId();

};