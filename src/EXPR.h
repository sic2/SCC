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
	void generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);

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
	void generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);
	void generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);
	void generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);
	void generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream);

	/*
	* BiOp subroutines
	*/
	void generateAdditionByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);
	void generateSubtractionByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);


};