#include "include/parser.hpp"
#include <iostream>

#include "binaryexpressionnode.hpp"
#include "boolliteralnode.hpp"
#include "breaknode.hpp"
#include "continuenode.hpp"
#include "expressionnode.hpp"
#include "integerliteralnode.hpp"
#include "whilestatementnode.hpp"

Parser::Parser(Lexer& lexer) : lexer(lexer), token(new Token()) {}


void Parser::expect(TokenType type) const
{
	if (token->getType() != type)
	{
		std::cerr << "Expected token " << static_cast<int>(type) << ", got " << static_cast<int>(token->getType()) << std::endl;
	}
}

PrimaryExpressionNode* Parser::parsePrimaryExpression()
{
	switch (token->getType())
	{
		case TokenType::TRUE:
			return new BoolLiteralNode(true);
		case TokenType::FALSE:
			return new BoolLiteralNode(false);
		case TokenType::INTEGER_LITERAL:
			return new IntegerLiteralNode(token->getValue());
	default:
			std::cerr << "Invalid literal" << std::endl;
			return nullptr;
	}
}

ExpressionNode* Parser::parseExpression()
{
	ExpressionNode* left = parsePrimaryExpression();
	token = lexer.next();
	ExpressionNode* right;
	switch (token->getType())
	{
		case TokenType::AND:
			token = lexer.next();
			right = parseExpression();
			left = new BinaryExpressionNode(left, right);
			break;
		case TokenType::OR:
			token = lexer.next();
			right = parseExpression();
			left = new BinaryExpressionNode(left, right);
			break;
	}
	return left;
}

LoopStatementNode* Parser::parseLoopStatement()
{
	token = lexer.next();
	const auto body = parseBlock();
	return new LoopStatementNode(body);
}

ReturnStatementNode* Parser::parseReturn()
{
	token = lexer.next();
	ExpressionNode* expression = nullptr;
	if (token->getType() != TokenType::SEMICOLON){
		expression = parseExpression();
	}
	expect(TokenType::SEMICOLON);
	token = lexer.next();
	return new ReturnStatementNode(expression);
}

IfStatementNode* Parser::parseIfStatement()
{
	token = lexer.next();
	expect(TokenType::PAR_OPEN);
	token = lexer.next();
	const auto condition = parseExpression();
	expect(TokenType::PAR_CLOSE);
	token = lexer.next();
	StatementNode* body = parseStatement();
	StatementNode* else_body = nullptr;
	if (token->getType() == TokenType::ELSE)
	{
		token = lexer.next();
		else_body = parseStatement();
	}
	return new IfStatementNode(condition, body, else_body);
}

DoWhileStatementNode* Parser::parseDoWhileStatement()
{
	token = lexer.next();
	const auto body = parseStatement();
	expect(TokenType::WHILE);
	token = lexer.next();
	expect(TokenType::PAR_OPEN);
	token = lexer.next();
	const auto condition = parseExpression();
	expect(TokenType::PAR_CLOSE);
	token = lexer.next();
	expect(TokenType::SEMICOLON);
	token = lexer.next();
	return new DoWhileStatementNode(body, condition);
}


SwitchStatementNode* Parser::parseSwitchStatement()
{
	token = lexer.next();
	expect(TokenType::PAR_OPEN);
	token = lexer.next();
	const auto eval_expression = parseExpression();
	expect(TokenType::PAR_CLOSE);
	token = lexer.next();

	std::map<ExpressionNode*, StatementNode*> cases;
	expect(TokenType::CURLY_OPEN);
	token = lexer.next();
	while (token->getType() != TokenType::CURLY_CLOSE)
	{
		if (token->empty())
		{
			std::cerr << "Unclosed brace" << std::endl;
			return nullptr;
		}

		ExpressionNode* expression = nullptr;
		if (token->getType() == TokenType::DEFAULT)
		{
			token = lexer.next();
		}
		else
		{
			expect(TokenType::CASE);
			token = lexer.next();
			expression = parseExpression();
		}
		expect(TokenType::COLON);
		token = lexer.next();
		const auto body = parseStatement();
		cases.insert(std::make_pair(expression, body));
	}
	token = lexer.next();
	return new SwitchStatementNode(eval_expression, cases);
}


WhileStatementNode* Parser::parseWhileStatement()
{
	token = lexer.next();
	expect(TokenType::PAR_OPEN);
	token = lexer.next();
	ExpressionNode* condition = parseExpression();
	expect(TokenType::PAR_CLOSE);
	token = lexer.next();
	StatementNode* body = parseStatement();
	return new WhileStatementNode(condition, body);
}

StatementNode* Parser::parseStatement()
{
	switch (token->getType())
	{
		case TokenType::CURLY_OPEN:
			return parseBlock();
		case TokenType::LOOP:
			return parseLoopStatement();
		case TokenType::RETURN:
			return parseReturn();
		case TokenType::BREAK:
			token = lexer.next();
			expect(TokenType::SEMICOLON);
			token = lexer.next();
			return new BreakNode();
		case TokenType::CONTINUE:
			token = lexer.next();
			expect(TokenType::SEMICOLON);
			token = lexer.next();
			return new ContinueNode();
		case TokenType::IF:
			return parseIfStatement();
		case TokenType::WHILE:
			return parseWhileStatement();
		case TokenType::DO:
			return parseDoWhileStatement();
		case TokenType::SWITCH:
			return parseSwitchStatement();
	default:
			std::cerr << "Invalid statement" << *token << std::endl;
			return nullptr;
	}
}


BlockNode* Parser::parseBlock()
{
	expect(TokenType::CURLY_OPEN);
	token = lexer.next();
	std::vector<ASTNode*> statements;
	while (token->getType() != TokenType::CURLY_CLOSE)
	{
		if (token->empty())
		{
			std::cerr << "Unclosed brace" << std::endl;
			return nullptr;
		}
		statements.push_back(parseStatement());
	}
	token = lexer.next();
	return new BlockNode(statements);
}


FunctionDeclarationNode* Parser::parseFunctionDeclaration()
{
	token = lexer.next();
	expect(TokenType::IDENTIFIER);
	const auto name = token->getValue();
	token = lexer.next();
	expect(TokenType::PAR_OPEN);
	token = lexer.next();
	expect(TokenType::PAR_CLOSE);
	token = lexer.next();

	std::string type;
	if (token->getType() == TokenType::COLON)
	{
		token = lexer.next();
		expect(TokenType::IDENTIFIER);
		type = token->getValue();
		token = lexer.next();
	}
	BlockNode* body = parseBlock();
	return new FunctionDeclarationNode(name, type, body);
}

VarDeclarationNode* Parser::parseVarDeclaration(const bool is_const)
{
	token = lexer.next();
	expect(TokenType::IDENTIFIER);
	const auto name = token->getValue();
	token = lexer.next();

	std::string type;
	if (token->getType() == TokenType::COLON)
	{
		token = lexer.next();
		expect(TokenType::IDENTIFIER);
		type = token->getValue();
		token = lexer.next();
	}
	ExpressionNode* expr = nullptr;
	if (token->getType() == TokenType::ASSIGN)
	{
		token = lexer.next();
		expr = parseExpression();
	}
	expect(TokenType::SEMICOLON);
	token = lexer.next();
	return new VarDeclarationNode(name, type, is_const, expr);
}

ASTNode* Parser::parseGlobalStatement()
{
	switch (token->getType())
	{
		case TokenType::VAR:
			return parseVarDeclaration(false);
		case TokenType::CONST:
			return parseVarDeclaration(true);
		case TokenType::DEF:
			return parseFunctionDeclaration();
		default:
			std::cerr << "Invalid global statement " << std::endl;
			return nullptr;
	}
}

RootNode* Parser::parse()
{
	const auto root = new RootNode();
	token = lexer.next();
	while (!token->empty())
	{
		root->addChild(parseGlobalStatement());
	}
	return root;
}


