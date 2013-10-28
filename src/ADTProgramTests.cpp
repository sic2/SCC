#include "ADTProgramTests.h"

boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest0()
{
	std::vector< boost::shared_ptr<AST::EXPR> > expressions;

	// Typedef for Time
	std::vector< boost::shared_ptr<AST::TYPE> > intTypes;
	intTypes.push_back(type_INT);
	boost::shared_ptr<AST::CONSTR> constrHour(new CONSTR("Hour", intTypes));
	boost::shared_ptr<AST::CONSTR> constrMin(new CONSTR("Min", intTypes));

	std::vector< boost::shared_ptr<AST::CONSTR> > constructors;
	constructors.push_back(constrHour);
	constructors.push_back(constrMin);

	Expr_Typedef typedefTime("Time", constructors); // type Time = Hour int | Min int
	boost::variant< Expr_Typedef > value_typedefTime(typedefTime);
	boost::shared_ptr<AST::EXPR> typeDefExpr(new EXPR(EXPR_TYPE_DEF, value_typedefTime));
	expressions.push_back(typeDefExpr);

	// Creating a new variable of type Time
	Expr_New_Var exprNewVar("t", "Time", "Min", expr_ZERO); // t Time :: Min 0;
	boost::variant< Expr_New_Var > newVar(exprNewVar);
	boost::shared_ptr<AST::EXPR> newVarExpr(new EXPR(EXPR_NEW_VAR, newVar));
	expressions.push_back(newVarExpr);

	Expr_Group exprGroup(expressions);
	boost::variant< Expr_Group > value_group(exprGroup);
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_GROUP, value_group));
	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(mainExpr));

	return program_0;
}

boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest1()
{
	std::vector< boost::shared_ptr<AST::EXPR> > expressions;

	// Typedef for Age
	std::vector< boost::shared_ptr<AST::TYPE> > intTypes;
	intTypes.push_back(type_INT);
	boost::shared_ptr<AST::CONSTR> constrAge(new CONSTR("Age", intTypes));
	std::vector< boost::shared_ptr<AST::CONSTR> > constructorsAge;
	constructorsAge.push_back(constrAge);

	Expr_Typedef typedefAge("Age", constructorsAge);
	boost::variant< Expr_Typedef > value_typedefAge(typedefAge);
	boost::shared_ptr<AST::EXPR> typeDefExpr(new EXPR(EXPR_TYPE_DEF, value_typedefAge));
	expressions.push_back(typeDefExpr);

	// Typedef for Addr
	std::vector< boost::shared_ptr<AST::TYPE> > typesAddr;
	typesAddr.push_back(type_INT);
	typesAddr.push_back(type_STRING);
	boost::shared_ptr<AST::CONSTR> constrAddress(new CONSTR("Address", typesAddr));
	std::vector< boost::shared_ptr<AST::CONSTR> > constructorsAddr;
	constructorsAddr.push_back(constrAddress);

	Expr_Typedef typedefAddr("Address", constructorsAddr);
	boost::variant< Expr_Typedef > value_typedefAddr(typedefAddr);
	boost::shared_ptr<AST::EXPR> typeDefExprAddr(new EXPR(EXPR_TYPE_DEF, value_typedefAddr));
	expressions.push_back(typeDefExprAddr);

	// Typedef for Person
	std::vector< boost::shared_ptr<AST::TYPE> > noTypes;
	std::vector< boost::shared_ptr<AST::TYPE> > subTypes;
	boost::shared_ptr<TYPE> type_Age(new TYPE("Age", noTypes));
	boost::shared_ptr<TYPE> type_Address(new TYPE("Address", noTypes));
	subTypes.push_back(type_Age);
	subTypes.push_back(type_Address);

	boost::shared_ptr<AST::CONSTR> constrPerson(new CONSTR("Person", subTypes));
	std::vector< boost::shared_ptr<AST::CONSTR> > constructorsPerson;
	constructorsPerson.push_back(constrPerson);

	Expr_Typedef typedefPerson("Person", constructorsPerson);
	boost::variant< Expr_Typedef > value_typedefPerson(typedefPerson);
	boost::shared_ptr<AST::EXPR> typeDefExprPerson(new EXPR(EXPR_TYPE_DEF, value_typedefPerson));
	expressions.push_back(typeDefExprPerson);

	// Creating a new variable of type Person
	std::vector< boost::shared_ptr<AST::EXPR> > personParams;

	// AGE
	std::vector< boost::shared_ptr<AST::EXPR> > ageParams;
	ageParams.push_back(expr_TWO); // Age = 2 // TODO - change to > 18
	Expr_Group groupParamsAge(ageParams);
	boost::variant< Expr_Group > value_groupParamsAge(groupParamsAge);
	boost::shared_ptr<AST::EXPR> parametersAge(new EXPR(EXPR_GROUP, value_groupParamsAge));

	Expr_New_Var ageNewVar("Age", "Age", "Age", parametersAge);
	boost::variant< Expr_New_Var > value_ageVar(ageNewVar);
	boost::shared_ptr<AST::EXPR> ageVarExpr(new EXPR(EXPR_NEW_VAR, value_ageVar));
	personParams.push_back(ageVarExpr);

	// ADDRESS
	std::vector< boost::shared_ptr<AST::EXPR> > addrParams;
	addrParams.push_back(expr_ZERO);
	addrParams.push_back(expr_STR);
	Expr_Group groupParamsAddr(addrParams); // Address = 0 "String"
	boost::variant< Expr_Group > value_groupParamsAddr(groupParamsAddr);
	boost::shared_ptr<AST::EXPR> parametersAddr(new EXPR(EXPR_GROUP, value_groupParamsAddr));

	Expr_New_Var addrNewVar("Address", "Address", "Address", parametersAddr);
	boost::variant< Expr_New_Var > value_addrVar(addrNewVar);
	boost::shared_ptr<AST::EXPR> addrVarExpr(new EXPR(EXPR_NEW_VAR, value_addrVar));
	personParams.push_back(addrVarExpr);

	// PERSON
	Expr_Group groupParams(personParams);
	boost::variant< Expr_Group > value_groupParams(groupParams);
	boost::shared_ptr<AST::EXPR> parameters(new EXPR(EXPR_GROUP, value_groupParams));
	Expr_New_Var exprNewVar("kevin", "Person", "Person", parameters); // kevin Person :: Person (Age 2) (Address 10 "street")
	boost::variant< Expr_New_Var > newVar(exprNewVar);
	boost::shared_ptr<AST::EXPR> newVarExpr(new EXPR(EXPR_NEW_VAR, newVar));
	expressions.push_back(newVarExpr);

	Expr_Group exprGroup(expressions);
	boost::variant< Expr_Group > value_group(exprGroup);
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_GROUP, value_group));
	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(mainExpr));

	return program_0;
}

// boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest2()
// {
// 	// TODO
// }