#pragma once

#include "AST.h"
#include "JVMByteCodeGenerator.h"

class AST::PROGRAM 
{
public:
	PROGRAM(boost::shared_ptr<AST::EXPR>* expr);
	virtual ~PROGRAM() {}

	/**
	* Traverse the AST tree and transform it
	* in Jasmin bytecode.
	* @param [out] jasminProgram represent the entire jasmin bytecode program, 
	*				excluding the main method. 
	*				Additional methods should be added to it.
	* @param [out] mainMethod used for the bytecode of the main method of the program.
	* @note The mainMethod should added at the end of the jasminProgram by the caller.
	*/
	void generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod);

private:
	boost::shared_ptr<AST::EXPR>* _expr;
};