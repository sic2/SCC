#include "basicProgramTests.h"

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest0()
{
	Expr_Bi_Op exprBiOp(expr_ZERO, op_add, expr_ONE);
	boost::variant< Expr_Bi_Op > value_3(exprBiOp);

	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_BI_OP, value_3));
	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(mainExpr));

	return program_0;
}

// FIXME - test is broken
boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest1()
{
	std::vector< boost::shared_ptr<ALT> > alternatives;
	alternatives.push_back(alt_ZERO);
	alternatives.push_back(alt_ONE);

	Expr_Case exprCase(expr_ONE, alternatives);
	boost::variant< Expr_Case > value_3(exprCase);

	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_CASE, value_3));
	boost::shared_ptr<AST::PROGRAM> program_1(new PROGRAM(mainExpr));

	return program_1;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest2()
{
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_INT, value_ONE));
	boost::shared_ptr<AST::PROGRAM> program_2(new PROGRAM(mainExpr));

	return program_2;
}

/******************
* Some previous examples
*******************/

// uValue value_3 = {
// 				exprVarConstr: 
// 					{
// 					ID: new std::string("test"), 
// 					expr: &expr_0
// 					}
// 				};

// uValue value_4 = {
// 				exprCase: 
// 					{
// 					expr: &expr_1, 
// 					alternatives: &alternatives // XXX - does this still work even if alternatives are pushed later?!
// 					}
// 				};

// /*
// * 0 + 1
// */
// uValue value_5 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_0,
// 					op: &op_add,
// 					expr1: &expr_1
// 					}
// 				};

// /*
// * testVar :: INT = 0
// */
// uValue value_6 = {
// 				exprNewVar:
// 					{
// 					ID: new std::string("testVar"),
// 					expr: &expr_1,
// 					}
// 				};

// /*
// * (1 - 2)
// */
// uValue value_7 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_1,
// 					op: &op_sub,
// 					expr1: &expr_2
// 					}
// 				};

// /*
// * 1 * 2
// */
// uValue value_8 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_1,
// 					op: &op_mul,
// 					expr1: &expr_2
// 					}
// 				};

// /*
// * 2 / 1
// */
// uValue value_9 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_2,
// 					op: &op_div,
// 					expr1: &expr_1
// 					}
// 				};

// /*
// * 2 < 1
// */
// uValue value_10 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_2,
// 					op: &op_less_than,
// 					expr1: &expr_1
// 					}
// 				};

// /*
// * 2 == 1
// */
// uValue value_11 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_2,
// 					op: &op_eq,
// 					expr1: &expr_1
// 					}
// 				};

// /*
// * true or false
// */
// uValue value_12 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_TRUE,
// 					op: &op_or,
// 					expr1: &expr_FALSE
// 					}
// 				};


// /*
// * true and false
// */
// uValue value_13 = {
// 				exprBiOp:
// 					{
// 					expr: &expr_TRUE,
// 					op: &op_and,
// 					expr1: &expr_FALSE
// 					}
// 				};
