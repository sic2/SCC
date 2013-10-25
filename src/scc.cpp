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

#include "OPERATOR.h"
#include "ALT.h"
#include "EXPR.h"
#include "PROGRAM.h"
#include "TYPE.h"


#include "JVMByteCodeGenerator.h"

using namespace AST;

/*
* Define values
*/
uValue value_TRUE = {
				Bool: true
				};

uValue value_FALSE = {
				Bool: false
				};

uValue value_0 = {
				Integer: 0
				};
uValue value_1 = {
				Integer: 1
				};

uValue value_2 = {
				Integer: 2
				};

/*
* Operators
*/
boost::shared_ptr<OPERATOR> op_add(new OPERATOR(OP_ADDITION));
boost::shared_ptr<OPERATOR> op_sub(new OPERATOR(OP_SUBTRACTION));
boost::shared_ptr<OPERATOR> op_mul(new OPERATOR(OP_MULTIPLICATION));
boost::shared_ptr<OPERATOR> op_div(new OPERATOR(OP_DIVISION));
boost::shared_ptr<OPERATOR> op_eq(new OPERATOR(OP_EQUALITY));
boost::shared_ptr<OPERATOR> op_less_than(new OPERATOR(OP_LESS));
boost::shared_ptr<OPERATOR> op_or(new OPERATOR(OP_OR));
boost::shared_ptr<OPERATOR> op_and(new OPERATOR(OP_AND));

boost::shared_ptr<EXPR> expr_TRUE(new EXPR(EXPR_BOOL, value_TRUE));
boost::shared_ptr<EXPR> expr_FALSE(new EXPR(EXPR_BOOL, value_FALSE));

boost::shared_ptr<EXPR> expr_0(new EXPR(EXPR_INT, value_0));
boost::shared_ptr<EXPR> expr_1(new EXPR(EXPR_INT, value_1));
boost::shared_ptr<EXPR> expr_2(new EXPR(EXPR_INT, value_2));


boost::shared_ptr<TYPE> type_0(new TYPE(TYPE_INT));
boost::shared_ptr<TYPE> type_1(new TYPE(TYPE_INT));

// Create alternatives one by one and add them to vector of alternatives
std::vector< boost::shared_ptr<ALT> > alternatives;
boost::shared_ptr<ALT> alt_0(new ALT(&expr_0, &expr_1)); 
boost::shared_ptr<ALT> alt_1(new ALT(&expr_1, &expr_0)); 

uValue value_3 = {
				exprVarConstr: 
					{
					ID: new std::string("test"), 
					expr: &expr_0
					}
				};

uValue value_4 = {
				exprCase: 
					{
					expr: &expr_1, 
					alternatives: &alternatives // XXX - does this still work even if alternatives are pushed later?!
					}
				};

/*
* 0 + 1
*/
uValue value_5 = {
				exprBiOp:
					{
					expr: &expr_0,
					op: &op_add,
					expr1: &expr_1
					}
				};

/*
* testVar :: INT = 0
*/
uValue value_6 = {
				exprNewVar:
					{
					ID: new std::string("testVar"),
					expr: &expr_1,
					}
				};

/*
* (1 - 2)
*/
uValue value_7 = {
				exprBiOp:
					{
					expr: &expr_1,
					op: &op_sub,
					expr1: &expr_2
					}
				};

/*
* 1 * 2
*/
uValue value_8 = {
				exprBiOp:
					{
					expr: &expr_1,
					op: &op_mul,
					expr1: &expr_2
					}
				};

/*
* 2 / 1
*/
uValue value_9 = {
				exprBiOp:
					{
					expr: &expr_2,
					op: &op_div,
					expr1: &expr_1
					}
				};

/*
* 2 < 1
*/
uValue value_10 = {
				exprBiOp:
					{
					expr: &expr_2,
					op: &op_less_than,
					expr1: &expr_1
					}
				};

/*
* 2 == 1
*/
uValue value_11 = {
				exprBiOp:
					{
					expr: &expr_2,
					op: &op_eq,
					expr1: &expr_1
					}
				};

/*
* true or false
*/
uValue value_12 = {
				exprBiOp:
					{
					expr: &expr_TRUE,
					op: &op_or,
					expr1: &expr_FALSE
					}
				};


/*
* true and false
*/
uValue value_13 = {
				exprBiOp:
					{
					expr: &expr_TRUE,
					op: &op_and,
					expr1: &expr_FALSE
					}
				};

boost::shared_ptr<EXPR> mainExpr(new EXPR(EXPR_BI_OP, value_13));
 //boost::shared_ptr<EXPR> mainExpr(new EXPR(EXPR_INT, value_2));
// boost::shared_ptr<EXPR> mainExpr(new EXPR(EXPR_CASE, value_4));

// Test programs are encoded and evaluated 
// in the main method.
// A 2-steps process is used:
// - Encode the program in AST
// - Generate JVM bytecode from the AST
// The JVM bytecode is printed to file if successful. 
int main(int argc, char** argv)
{
	alternatives.push_back(alt_0);
	alternatives.push_back(alt_1);

	/**************
	* TEST PROGRAMS
	***************/

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