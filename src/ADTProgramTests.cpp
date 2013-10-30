#include "ADTProgramTests.h"


boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest0()
{
	std::vector< boost::shared_ptr<AST::EXPR> > expressions;
	boost::shared_ptr<AST::EXPR> typeDefExpr = getTypedefExpr("Time", "ctct", "Hour", &type_INT, "Min", &type_INT);
	expressions.push_back(typeDefExpr);

	// Creating a new variable of type Time
	std::vector< boost::shared_ptr<AST::EXPR> > values;
	values.push_back(expr_ZERO);
	Expr_Var_Constr min("Min", values); // Min 0
	boost::variant< Expr_Var_Constr > value_min(min);
	boost::shared_ptr<AST::EXPR> minExpr(new EXPR(EXPR_VAR_CONSTR, value_min));


	Expr_New_Var exprNewVar("t", "Time", minExpr); // t Time :: Min 0
	boost::variant< Expr_New_Var > newVar(exprNewVar);
	boost::shared_ptr<AST::EXPR> newVarExpr(new EXPR(EXPR_NEW_VAR, newVar));
	expressions.push_back(newVarExpr);

	Expr_Group exprGroup(expressions);
	boost::variant< Expr_Group > value_group(exprGroup);
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_GROUP, value_group));
	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(mainExpr));

	return program_0;
}

/*
type Age = Age int; 
type Address = Addr int string; 
type Person = Person Age Address; 
kevin :: Person = Person { Age 21 } { Address 1 "..." };
case kevin of
	Person agek addr -> agek
*/
boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest1()
{
	std::vector< boost::shared_ptr<AST::EXPR> > expressions;

	boost::shared_ptr<AST::EXPR> ageTypeDefExpr = getTypedefExpr("Age", "ct", "Age", &type_INT);
	expressions.push_back(ageTypeDefExpr);

	boost::shared_ptr<AST::EXPR> addrTypeDefExpr = getTypedefExpr("Address", "ctt", "Address", &type_INT, &type_STRING);
	expressions.push_back(addrTypeDefExpr);

	// Typedef for Person
	std::vector< boost::shared_ptr<AST::TYPE> > noTypes;
	boost::shared_ptr<TYPE> type_Age(new TYPE("Age", noTypes));
	boost::shared_ptr<TYPE> type_Address(new TYPE("Address", noTypes));
	boost::shared_ptr<AST::EXPR> personTypeDefExpr = getTypedefExpr("Person", "ctt", "Person", &type_Age, &type_Address);
	expressions.push_back(personTypeDefExpr);

	// Creating a new variable of type Person
	std::vector< boost::shared_ptr<AST::EXPR> > personParams;

	// AGE
	std::vector< boost::shared_ptr<AST::EXPR> > ageParams;
	ageParams.push_back(expr_21); // Age = 21
	Expr_Var_Constr age("Age", ageParams);
	boost::variant< Expr_Var_Constr > value_age(age);
	boost::shared_ptr<AST::EXPR> ageExpr(new EXPR(EXPR_VAR_CONSTR, value_age));

	Expr_New_Var ageNewVar("Age", "Age", ageExpr);
	boost::variant< Expr_New_Var > value_ageVar(ageNewVar);
	boost::shared_ptr<AST::EXPR> ageVarExpr(new EXPR(EXPR_NEW_VAR, value_ageVar));
	personParams.push_back(ageVarExpr);

	// ADDRESS
	std::vector< boost::shared_ptr<AST::EXPR> > addrParams;
	addrParams.push_back(expr_ZERO);
	addrParams.push_back(expr_STR);
	Expr_Var_Constr address("Address", addrParams); // Address = 0 "String"
	boost::variant< Expr_Var_Constr > value_address(address);
	boost::shared_ptr<AST::EXPR> parametersAddr(new EXPR(EXPR_VAR_CONSTR, value_address));

	Expr_New_Var addrNewVar("Address", "Address", parametersAddr);
	boost::variant< Expr_New_Var > value_addrVar(addrNewVar);
	boost::shared_ptr<AST::EXPR> addrVarExpr(new EXPR(EXPR_NEW_VAR, value_addrVar));
	personParams.push_back(addrVarExpr);

	// PERSON
	Expr_Var_Constr ppParams("Person", personParams);
	boost::variant< Expr_Var_Constr > value_person(ppParams);
	boost::shared_ptr<AST::EXPR> parameters(new EXPR(EXPR_VAR_CONSTR, value_person));
	
	Expr_New_Var exprNewVar("kevin", "Person", parameters); // kevin Person :: Person (Age 21) (Address 0 "String")
	boost::variant< Expr_New_Var > newVar(exprNewVar);
	boost::shared_ptr<AST::EXPR> newVarExpr(new EXPR(EXPR_NEW_VAR, newVar));
	expressions.push_back(newVarExpr);

	// Case statement
	/*
	case Kevin of
		Person agek addr -> agek
	*/
	std::vector< boost::shared_ptr<AST::EXPR> > values;
	Expr_Var_Constr kevin("kevin", values);
	boost::variant< Expr_Var_Constr > value_kevin(kevin);
	boost::shared_ptr<AST::EXPR> kevinExpr(new EXPR(EXPR_VAR_CONSTR, value_kevin));

	// Alternatives
	std::vector< boost::shared_ptr<TYPE> > personTypeParams;
	boost::shared_ptr<TYPE> agek(new TYPE("agek", noTypes));
	boost::shared_ptr<TYPE> addr(new TYPE("addr", noTypes));
	personTypeParams.push_back(agek);
	personTypeParams.push_back(addr);
	boost::shared_ptr<TYPE> type_Person(new TYPE("Person", personTypeParams)); // Person agek addr
	boost::shared_ptr<ALT> alt_Person(new ALT(type_Person, expr_ONE)); 

	std::vector< boost::shared_ptr<ALT> > alternatives;
	alternatives.push_back(alt_Person);

	// Stitch case condition and alternatives together
	Expr_Case exprCase(kevinExpr, alternatives, AST::EXPR_INT);
	boost::variant< Expr_Case > value_3(exprCase);

	boost::shared_ptr<AST::EXPR> caseExpr(new EXPR(EXPR_CASE, value_3));
	expressions.push_back(caseExpr);

	Expr_Group exprGroup(expressions);
	boost::variant< Expr_Group > value_group(exprGroup);
	boost::shared_ptr<AST::EXPR> mainExpr(new EXPR(EXPR_GROUP, value_group));
	boost::shared_ptr<AST::PROGRAM> program_0(new PROGRAM(mainExpr));

	return program_0;
}