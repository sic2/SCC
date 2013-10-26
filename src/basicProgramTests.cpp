#include "basicProgramTests.h"

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getIt()
{
	boost::shared_ptr<OPERATOR> op_add(new OPERATOR(OP_ADDITION));

	int int10 = 10;
	boost::variant< int > value_0(int10);
	int int2 = 2;
	boost::variant< int > value_1(int2);

	boost::shared_ptr<EXPR> expr_0(new EXPR(EXPR_INT, value_0));
	boost::shared_ptr<EXPR> expr_1(new EXPR(EXPR_INT, value_1));

	Expr_Bi_Op exprBiOp(expr_0, op_add, expr_1);
	boost::variant< Expr_Bi_Op > value_3(exprBiOp);

	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_BI_OP, value_3));
	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(mainExpr));

	return program_0;
}



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

// boost::shared_ptr<EXPR> mainExpr(new EXPR(EXPR_BI_OP, value_13));
 //boost::shared_ptr<EXPR> mainExpr(new EXPR(EXPR_INT, value_2));
// boost::shared_ptr<EXPR> mainExpr(new EXPR(EXPR_CASE, value_4));


	// alternatives.push_back(alt_0);
	// alternatives.push_back(alt_1);