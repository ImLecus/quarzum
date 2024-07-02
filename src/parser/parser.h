/*
 * Quarzum Compiler - parser.h
 * Version 1.0, 02/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */


#pragma once
#include "../Quarzum.h"
#include "./node.cpp"
using namespace Quarzum::Debug;

#define EXPECT(cToken,cType, ErrorMessage) if(cToken.type != cType){throwError(ErrorMessage, cToken); return nullptr;}
#define IDENTATION(iName) while(not ask(right_curly)){if(tokens.empty()){err("Mismatched '{'");return nullptr;}iName->nodes.push_back(parseStatement());}tokens.pop_front();

class Parser {
public:
    Parser(const std::deque<Token>& tokens);
    const std::unique_ptr<RootNode> parse();
private:
    std::deque<Token> tokens;
    inline const Token pop() noexcept;
    inline const bool ask(const TokenType t) const noexcept;
    std::unique_ptr<Statement> parseStatement();

    // STATEMENT PARSING RELATED
    std::unique_ptr<ReturnStatement> parseReturn();
    std::unique_ptr<BreakStatement> parseBreak();
    std::unique_ptr<ContinueStatement> parseContinue();
    std::unique_ptr<ForeignStatement> parseForeign();
    /**
     * Returns a VarDeclaration or redirects to parseFunction();
     */
    std::unique_ptr<Statement> parseVar(const bool isConst = false, const std::string& access = "none");
    std::unique_ptr<VarRedec> parseRedec(const Token& id, const bool isPrefix = false);
    std::unique_ptr<EnumStatement> parseEnum();
    std::unique_ptr<FunctionCall> parseFunctionCall(const Token& id);
    std::unique_ptr<DeleteStatement> parseDelete();
    std::unique_ptr<ImportStatement> parseImport();

    // CONTAINER PARSING RELATED
    std::unique_ptr<ModuleContainer> parseModule();
    std::unique_ptr<IfContainer> parseIf();
    std::unique_ptr<WhileContainer> parseWhile();
    std::unique_ptr<DoWhileContainer> parseDoWhile();
    std::unique_ptr<ForeachContainer> parseForeach();
    std::unique_ptr<ClassContainer> parseClass();
    std::unique_ptr<ForContainer> parseFor();
    std::unique_ptr<FunctionContainer> parseFunction();
    // setup() and destroy() special functions
    // "parseMethod" == access + parseFunction

    // EXPRESSION PARSING RELATED

    /**
     * @brief Parses a expression. Returns nullptr if something goes wrong.
     */
    std::unique_ptr<Expr> parseExpr();
    /**
     * @brief Parses a primary expression 
     * (function call, identifier, literal, unary expressions or array indexation)
     */
    std::unique_ptr<Expr> parsePrimaryExpr();
    /**
     * @brief Parses a expression between parentheses.
     */
    std::unique_ptr<ParenExpr> parseParenExpr();
    /**
     * @brief Parses function calls inside an expression.
     */
    std::unique_ptr<FunctionCallExpr> parseFunctionCallExpr(Token id);

    // OTHER

    // getIdentifier
    // getType
    // parseArgs?
};