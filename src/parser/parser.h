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
    const unique_ptr<RootNode> parse();
private:
    std::deque<Token> tokens;
    inline const Token pop() noexcept;
    inline const bool ask(const TokenType t) const noexcept;
    unique_ptr<Statement> parseStatement();

    // STATEMENT PARSING RELATED
    unique_ptr<ReturnStatement> parseReturn();
    unique_ptr<BreakStatement> parseBreak();
    unique_ptr<ContinueStatement> parseContinue();
    unique_ptr<ForeignStatement> parseForeign();
    /**
     * Returns a VarDeclaration or redirects to parseFunction();
     */
    unique_ptr<Statement> parseVar(const bool isConst = false, const string& access = "none");
    unique_ptr<VarRedec> parseRedec(const Token& id, const bool isPrefix = false);
    unique_ptr<EnumStatement> parseEnum();
    unique_ptr<FunctionCall> parseFunctionCall(const Token& id);
    unique_ptr<DeleteStatement> parseDelete();
    unique_ptr<ImportStatement> parseImport();

    // CONTAINER PARSING RELATED
    unique_ptr<ModuleContainer> parseModule();
    unique_ptr<IfContainer> parseIf();
    unique_ptr<WhileContainer> parseWhile();
    unique_ptr<DoWhileContainer> parseDoWhile();
    unique_ptr<ForeachContainer> parseForeach();
    unique_ptr<ClassContainer> parseClass();
    unique_ptr<ForContainer> parseFor();
    unique_ptr<FunctionContainer> parseFunction();
    // setup() and destroy() special functions
    // "parseMethod" == access + parseFunction

    // EXPRESSION PARSING RELATED

    /**
     * @brief Parses a expression. Returns nullptr if something goes wrong.
     */
    unique_ptr<Expr> parseExpr();
    /**
     * @brief Parses a primary expression 
     * (function call, identifier, literal, unary expressions or array indexation)
     */
    unique_ptr<Expr> parsePrimaryExpr();
    /**
     * @brief Parses a expression between parentheses.
     */
    unique_ptr<ParenExpr> parseParenExpr();
    /**
     * @brief Parses function calls inside an expression.
     */
    unique_ptr<FunctionCallExpr> parseFunctionCallExpr(Token id);

    // OTHER

    string getIdentifier();
    // getIdentifier
    // getType
    // parseArgs?
};