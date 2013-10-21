#pragma once

#include "AST.h"

class JVMByteCodeGenerator
{
public:
	JVMByteCodeGenerator(AST::PROGRAM& program): _program(program) {}

	/**
	* Generates JVM bytecode and output to file
	* @param outFileName 
	* @return 	True if bytecode was generate successfully.
	*			False otherwise.
	*/
	bool generateByteCode(std::string outFileName);

private:
	AST::PROGRAM& _program;
};