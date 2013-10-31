#include "basicProgramTests.h"

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest0()
{
	boost::shared_ptr<AST::EXPR> mainExpr = getBiOpExpr(expr_ZERO, AST::OP_ADDITION, expr_ONE);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}

/*
case 1
	0 -> 1 (alt_zero)
	1 -> 0 (alt_one)
*/
boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest1()
{
	boost::shared_ptr<AST::EXPR> mainExpr = getCaseExpr(AST::EXPR_INT, "1", "aa", &alt_ZERO, &alt_ONE);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest2()
{
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_INT, value_ONE));
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest3()
{
	boost::shared_ptr<AST::EXPR> addition = getBiOpExpr(expr_ZERO, AST::OP_ADDITION, expr_ONE);
	boost::shared_ptr<AST::EXPR> multiplication = getBiOpExpr(addition, AST::OP_MULTIPLICATION, expr_TWO);
	
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(multiplication));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest4()
{
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_BOOL, value_TRUE));
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest5()
{
	boost::shared_ptr<AST::EXPR> mainExpr = getBiOpExpr(expr_TRUE, AST::OP_AND, expr_FALSE);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest6()
{
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_STRING, value_STRING));
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest7()
{
	boost::shared_ptr<AST::EXPR> xExpr = getAssignmentExpr("x", expr_THREE);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(xExpr));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest8()
{
	boost::shared_ptr<AST::EXPR> mainExpr = getBiOpExpr(expr_THREE, AST::OP_LESS, expr_ONE);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}

boost::shared_ptr<AST::PROGRAM> basicProgramTests::getTest9()
{
	boost::shared_ptr<AST::EXPR> xExpr = getAssignmentExpr("x", expr_THREE);
	boost::shared_ptr<AST::EXPR> yExpr = getAssignmentExpr("y", expr_THREE);
	
	boost::shared_ptr<AST::EXPR> mainExpr = getBiOpExpr(xExpr, AST::OP_ADDITION, yExpr);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(mainExpr));
	return program;
}
