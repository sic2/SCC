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
	uValue value_0 = {
					Integer: 0
					};
	uValue value_1 = {
					Integer: 1
					};

	boost::shared_ptr<EXPR> expr_0(new EXPR(EXPR_INT, value_0));
	boost::shared_ptr<EXPR> expr_1(new EXPR(EXPR_INT, value_1));

	boost::shared_ptr<TYPE> type_0(new TYPE(TYPE_INT, value_0));
	boost::shared_ptr<TYPE> type_1(new TYPE(TYPE_INT, value_1));

	// Create alternatives one by one and add them to vector of alternatives
	std::vector< boost::shared_ptr<ALT> > alternatives;
	boost::shared_ptr<ALT> alt_0(new ALT(&type_0, &expr_1)); 
	boost::shared_ptr<ALT> alt_1(new ALT(&type_1, &expr_0)); 
	alternatives.push_back(alt_0);
	alternatives.push_back(alt_1);

	uValue value_2 = {
					Integer: 2
					};

	uValue value_3 = {
					exprVarConstr: 
						{
						ID: new std::string(""), 
						expr: &expr_0
						}
					};

	uValue value_4 = {
					exprCase: 
						{
						expr: &expr_1, 
						alternatives: &alternatives
						}
					};

	uValue value_5 = {
					exprBiOp:
						{
						expr: &expr_0,
						op: OP_ADDITION,
						expr1: &expr_1
						}
					};

	boost::shared_ptr<EXPR> mainExpr(new EXPR(EXPR_CASE, value_4));

	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(&mainExpr));
	JVMByteCodeGenerator byteCodeGen(program_0);
	byteCodeGen.generateByteCode("XXX");

	/*
	* Define test program
	*/
	//PROGRAM program_0(new EXPR(EXPR_BI_OP, value_5));
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