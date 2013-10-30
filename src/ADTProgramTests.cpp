#include "ADTProgramTests.h"

boost::shared_ptr<AST::PROGRAM> ADTProgramTests::getTest0()
{
	std::vector< boost::shared_ptr<AST::EXPR> > expressions;
	boost::shared_ptr<AST::EXPR> typeDefExpr = getTypedefExpr("Time", "ctct", "Hour", &type_INT, "Min", &type_INT);

	// Creating a new variable of type Time
	boost::shared_ptr<AST::EXPR> timeExpr = getNewVarExpr("t", "Time", "Min", "e", &expr_ZERO);
	
	boost::shared_ptr<AST::EXPR> expr = getGroupExpr("ee", &typeDefExpr, &timeExpr);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(expr));

	return program;
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

	// Typedefs for Age and Address
	boost::shared_ptr<AST::EXPR> ageTypeDefExpr = getTypedefExpr("Age", "ct", "Age", &type_INT);
	boost::shared_ptr<AST::EXPR> addrTypeDefExpr = getTypedefExpr("Address", "ctt", "Address", &type_INT, &type_STRING);

	// Typedef for Person
	std::vector< boost::shared_ptr<AST::TYPE> > noTypes;
	boost::shared_ptr<TYPE> type_Age(new TYPE("Age", noTypes));
	boost::shared_ptr<TYPE> type_Address(new TYPE("Address", noTypes));
	boost::shared_ptr<AST::EXPR> personTypeDefExpr = getTypedefExpr("Person", "ctt", "Person", &type_Age, &type_Address);

	// Creating a new variable of type Person
	boost::shared_ptr<AST::EXPR> ageExpr = getNewVarExpr("Age", "Age", "Age", "e", &expr_21);
	boost::shared_ptr<AST::EXPR> addrExpr = getNewVarExpr("Address", "Address", "Address", "ee", &expr_ZERO, &expr_STR);
	boost::shared_ptr<AST::EXPR> personExpr = getNewVarExpr("kevin", "Person", "Person", "ee", &ageExpr, &addrExpr);

	// Case statement
	/*
	case Kevin of
		Person agek addr -> agek
	*/
	// Alternatives
	std::vector< boost::shared_ptr<TYPE> > personTypeParams;
	boost::shared_ptr<TYPE> agek(new TYPE("agek", noTypes));
	boost::shared_ptr<TYPE> addr(new TYPE("addr", noTypes));
	personTypeParams.push_back(agek);
	personTypeParams.push_back(addr);
	boost::shared_ptr<TYPE> type_Person(new TYPE("Person", personTypeParams)); // Person agek addr
	boost::shared_ptr<ALT> alt_Person(new ALT(type_Person, expr_ONE)); 

	boost::shared_ptr<AST::EXPR> caseExpr = getCaseExpr(AST::EXPR_INT, "kevin", "a", &alt_Person);

	boost::shared_ptr<AST::EXPR> groupExpr = getGroupExpr("eeeee", &ageTypeDefExpr, &addrTypeDefExpr, &personTypeDefExpr, &personExpr, &caseExpr);
	boost::shared_ptr<AST::PROGRAM> program(new PROGRAM(groupExpr));

	return program;
}