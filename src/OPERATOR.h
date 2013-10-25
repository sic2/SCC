#pragma once

#include "AST.h"
#include "JVMByteCodeGenerator.h"

class AST::OPERATOR
{
public:

	OPERATOR(AST::OP op);
	virtual ~OPERATOR() {}

	/**
	* @param
	* @param
	* @param
	* @param
	* @return EXPRESSION_TYPE
	*/
	AST::EXPRESSION_TYPE generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, 
			std::string& mainMethod, AST::EXPRESSION_TYPE op0Type, AST::EXPRESSION_TYPE op1Type);

private:
	AST::OP _op;

	std::string getSubroutineParams(AST::EXPRESSION_TYPE op0Type, AST::EXPRESSION_TYPE op1Type);
	std::string getSubroutine(std::string& subroutineCallerName);
	std::string getSubroutineInitialisation(std::string& subroutineCallerName);
	std::string getSubroutineReturning();

	std::string getTrivialMathSubroutine();
	std::string getTrivialCmpSubroutine();
	std::string getTrivialLogicSubroutine();
};