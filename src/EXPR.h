#pragma once

#include "AST.h"
#include "JVMByteCodeGenerator.h"

class AST::EXPR
{
public:

	EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value);
	virtual ~EXPR();

	/*
	* Other methods
	*/ 
	void generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);

	AST::EXPRESSION_TYPE getExprType() { return this->_typeExpr; }
	AST::uValue getValue() { return this->_uValue; }

private:
	AST::EXPRESSION_TYPE _typeExpr;
	AST::uValue _uValue;

	std::string getIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, int Integer);

	void generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);

	void updateEnvironment(JVMByteCodeGenerator* bytecodeGenerator, std::string* ID, int index, AST::EXPRESSION_TYPE exprType);
};