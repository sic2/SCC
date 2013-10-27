#include "ADTProgramTests.h"

boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest0()
{
	// Typedefinition for Time
	std::vector< boost::shared_ptr<AST::TYPE> > intTypes;
	intTypes.push_back(type_INT);
	boost::shared_ptr<AST::CONSTR> constrHour(new CONSTR("Hour", intTypes));
	boost::shared_ptr<AST::CONSTR> constrMin(new CONSTR("Min", intTypes));

	std::vector< boost::shared_ptr<AST::CONSTR> > constructors;
	constructors.push_back(constrHour);
	constructors.push_back(constrMin);

	Expr_Typedef typedefTime("Time", constructors); // type Time = Hour int | Min int
	boost::variant< Expr_Typedef > value_typedefTime(typedefTime);

	// Creating a new variable of type Time
	Expr_New_Var exprNewVar("t", "Time", "Min", expr_ZERO); // t Time :: Min 0;
	boost::variant< Expr_New_Var > value_timeVar(exprNewVar);

	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_NEW_VAR, value_timeVar));
	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(mainExpr));

	return program_0;
}

// boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest1()
// {
// 	// TODO
// }

// boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest2()
// {
// 	// TODO
// }