/***********************************************************
* PLDI - A Compiler to JVM Bytecode: 
*				scc - St andrews Case Compiler 
* Student: 100003610
* 
* This software is part of University coursework and 
* can be reused only for teaching/learning purposes. 
* The code of the scc cannot be copied or reused in any 
* circumstances, unless permission has been granted by the author.
***********************************************************/
#include "JVMByteCodeGenerator.h"

using namespace AST;

// Test programs are encoded and evaluated 
// in the main method.
// A 2-steps process is used:
// - Encode the program in AST
// - Generate JVM bytecode from the AST
// The JVM bytecode is printed to file if successful. 
int main(int argc, char** argv)
{
	/*
	* Define values
	*/
	uValue expr_0 = {Integer: 0};
	uValue expr_1 = {Integer: 1};
	uValue expr_2 = {Integer: 2};

	// Create alternatives one by one and add them to vector of alternatives
	std::vector< boost::shared_ptr<ALT> > alternatives;
	boost::shared_ptr<ALT> alt_0(new ALT(new TYPE(TYPE_INT, expr_0), new EXPR(EXPR_INT, expr_1))); 
	boost::shared_ptr<ALT> alt_1(new ALT(new TYPE(TYPE_INT, expr_1), new EXPR(EXPR_INT, expr_0))); 
	alternatives.push_back(alt_0);
	alternatives.push_back(alt_1);

	PROGRAM program_0(new EXPR(EXPR_CASE, new EXPR(EXPR_INT, expr_0), alternatives));
	
	JVMByteCodeGenerator byteCodeGen(program_0);
	byteCodeGen.generateByteCode("XXX");


	/*
	* Define test program
	*/
	//PROGRAM program_0(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "+", new EXPR(EXPR_INT, expr)));
	// PROGRAM program_0(new EXPR(EXPR_INT, expr));
	//PROGRAM program_0(new EXPR());

	// Operand '&' is not defined in the Case language,
	// while the operands 'and' and '&&' are defined in the Case Language
	/*
	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "&", new EXPR(EXPR_INT, expr)));
	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "&&", new EXPR(EXPR_INT, expr)));
	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "and", new EXPR(EXPR_INT, expr)));
	*/
	return 0;
}