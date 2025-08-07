#pragma once
#include "dowhilestatementnode.hpp"
#include "functiondeclarationnode.hpp"
#include "ifstatementnode.hpp"
#include "lexer.hpp"
#include "loopstatementnode.hpp"
#include "primaryexpressionnode.hpp"
#include "returnstatementnode.hpp"
#include "rootnode.hpp"
#include "statementnode.hpp"
#include "switchstatementnode.hpp"
#include "vardeclarationnode.hpp"
#include "whilestatementnode.hpp"

class Parser
{
private:
	Lexer& lexer;
	const Token* token;

	void expect(TokenType) const;

	IfStatementNode* parseIfStatement();
	WhileStatementNode* parseWhileStatement();
	DoWhileStatementNode* parseDoWhileStatement();
	SwitchStatementNode* parseSwitchStatement();
	ReturnStatementNode* parseReturn();
	LoopStatementNode* parseLoopStatement();
	PrimaryExpressionNode* parsePrimaryExpression();
	ExpressionNode* parseExpression();
	ASTNode* parseGlobalStatement();
	StatementNode* parseStatement();
	BlockNode* parseBlock();
	FunctionDeclarationNode* parseFunctionDeclaration();
	VarDeclarationNode* parseVarDeclaration(bool);
public:
	explicit Parser(Lexer& lexer);

	RootNode* parse();
};
