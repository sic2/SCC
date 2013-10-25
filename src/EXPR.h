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
	/**
	* @parm printStream Set to true the first time
	*/
	EXPRESSION_TYPE generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);

	/*
	* Getters
	*/
	AST::EXPRESSION_TYPE getExprType() { return this->_typeExpr; }
	AST::uValue getValue() { return this->_uValue; }

private:
	/*
	* Utility functions
	*/
	std::string integerToString(int value);
	std::string getIStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator);

	AST::EXPRESSION_TYPE _typeExpr;
	AST::uValue _uValue;

	std::string getIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, int Integer);

	/*
	* Bytecode generators
	*/
	EXPRESSION_TYPE generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);
	EXPRESSION_TYPE generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);
	EXPRESSION_TYPE generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);
	EXPRESSION_TYPE generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);

	/*
	* BiOp subroutines
	*/
	void generateAdditionByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateSubtractionByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateMultiplicationByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateDivisionByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateLessThanByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateEqToByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);


};