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

#include <boost/shared_ptr.hpp>

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
	uValue expr;
	expr.Integer = -2;
	uValue expr_1;
	expr_1.Integer = 0;


	// In C++98 it is not possible to initialise a vector directly
	// as in C++11 (i.e. std::vector<int> v { 34,23 }; )
	// Therefore a 2steps initialisation is needed
	EXPR* e_0 = new EXPR(EXPR_INT, expr);
	boost::shared_ptr<ALT> alt(new ALT(new TYPE(TYPE_INT, expr_1), e_0)); 
	static const boost::shared_ptr<ALT> arr[] = 
	{
		alt
	};
	std::vector< boost::shared_ptr<ALT> > alternatives(arr, arr + sizeof(arr) / sizeof(arr[0]));

	
	// ALT(new TYPE(TYPE_INT), new EXPR(EXPR_INT, expr));

	/*
	* Define test program
	*/
	//PROGRAM program_0(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "+", new EXPR(EXPR_INT, expr)));
	// PROGRAM program_0(new EXPR(EXPR_INT, expr));
	PROGRAM program_0(new EXPR(EXPR_CASE, new EXPR(EXPR_INT, expr), alternatives));
	//PROGRAM program_0(new EXPR());
	JVMByteCodeGenerator byteCodeGen(program_0);
	byteCodeGen.generateByteCode("XXX");

	// Operand '&' is not defined in the Case language,
	// while the operands 'and' and '&&' are defined in the Case Language
	/*
	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "&", new EXPR(EXPR_INT, expr)));
	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "&&", new EXPR(EXPR_INT, expr)));
	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "and", new EXPR(EXPR_INT, expr)));
	*/
	return 0;
}