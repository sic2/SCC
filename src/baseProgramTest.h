#pragma once

#include <stdio.h>
#include <string>
#include <cstdarg>

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include "OPERATOR.h"
#include "ALT.h"
#include "EXPR.h"
#include "PROGRAM.h"
#include "TYPE.h"
#include "CONSTR.h"

using namespace AST;

// Define a constructor used by all the testing classes on loading
class baseProgramTest
{
public:
	inline baseProgramTest()
	{
		value_TRUE = boost::variant< bool >(true);
		value_FALSE = boost::variant< bool >(false);

		value_ZERO = boost::variant< int >(0);
		value_ONE = boost::variant< int >(1);
		value_TWO = boost::variant< int >(2);
		value_THREE = boost::variant< int >(3);
		value_21 = boost::variant< int >(21);

		value_STRING = boost::variant< std::string >("String");
		value_x_STR = boost::variant< std::string >("x");


		op_add = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_ADDITION));
		op_sub = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_SUBTRACTION));
		op_mul = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_MULTIPLICATION));
		op_div = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_DIVISION));
		op_eq = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_EQUALITY));
		op_less_than = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_LESS));
		op_or = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_OR));
		op_and = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_AND));
		op_range = boost::shared_ptr<OPERATOR> (new OPERATOR(OP_RANGE)); 


		expr_TRUE = boost::shared_ptr<EXPR> (new EXPR(EXPR_BOOL, value_TRUE));
		expr_FALSE = boost::shared_ptr<EXPR> (new EXPR(EXPR_BOOL, value_FALSE));

		expr_ZERO = boost::shared_ptr<EXPR> (new EXPR(EXPR_INT, value_ZERO));
		expr_ONE = boost::shared_ptr<EXPR> (new EXPR(EXPR_INT, value_ONE));
		expr_TWO = boost::shared_ptr<EXPR> (new EXPR(EXPR_INT, value_TWO));
		expr_THREE = boost::shared_ptr<EXPR> (new EXPR(EXPR_INT, value_THREE));
		expr_21 = boost::shared_ptr<EXPR> (new EXPR(EXPR_INT, value_21));

		expr_STR = boost::shared_ptr<EXPR> (new EXPR(EXPR_STRING, value_STRING));
		expr_x_STR = boost::shared_ptr<EXPR> (new EXPR(EXPR_STRING, value_x_STR));

		type_INT = boost::shared_ptr<TYPE> (new TYPE("int", noTypes));
		type_BOOL = boost::shared_ptr<TYPE> (new TYPE("bool", noTypes));
		type_STRING = boost::shared_ptr<TYPE> (new TYPE("string", noTypes));

		type_ZERO = boost::shared_ptr<TYPE> (new TYPE("0", noTypes));
		type_ONE = boost::shared_ptr<TYPE> (new TYPE("1", noTypes));


		alt_ZERO = boost::shared_ptr<ALT> (new ALT(type_ZERO, expr_ONE)); 
		alt_ONE = boost::shared_ptr<ALT> (new ALT(type_ONE, expr_ZERO)); 
	}

	virtual ~baseProgramTest() {}

	/*
  	* WRAPPERS 
	*/
  
	inline boost::shared_ptr<AST::EXPR> getBiOpExpr(boost::shared_ptr<AST::EXPR> operand0, OP op, boost::shared_ptr<AST::EXPR> operand1)
	{
		boost::shared_ptr<OPERATOR> retval;
		switch(op)
		{
		case AST::OP_ADDITION: retval = op_add;
			break;
		case AST::OP_SUBTRACTION: retval = op_sub;
			break;
		case AST::OP_MULTIPLICATION: retval = op_mul;
			break;
		case AST::OP_DIVISION: retval = op_div;
			break;
		case AST::OP_EQUALITY: retval = op_eq;
			break;
		case AST::OP_LESS: retval = op_less_than;
			break;
		case AST::OP_OR: retval = op_or;
			break;
		case AST::OP_AND: retval = op_and;
			break;
		case AST::OP_RANGE: retval = op_range;
			break;
		default:
			printf("Error: operation not supported\n");
		break;
		} // end switch

		Expr_Bi_Op exprBiOp(operand0, retval, operand1);
		boost::variant< Expr_Bi_Op > val(exprBiOp);
		boost::shared_ptr<AST::EXPR> expr(new EXPR(EXPR_BI_OP, val));
		
		return expr;
	}

	/**
  	* @param fmt
  				'a' for alternative
  	*/
	inline boost::shared_ptr<AST::EXPR> getCaseExpr(AST::EXPRESSION_TYPE type, std::string condition, const char *fmt, ...)
	{
		std::vector< boost::shared_ptr<AST::EXPR> > values;
		Expr_Var_Constr cond(condition, values);
		boost::variant< Expr_Var_Constr > value_cond(cond);
		boost::shared_ptr<AST::EXPR> condExpr(new EXPR(EXPR_VAR_CONSTR, value_cond));

		std::vector< boost::shared_ptr<ALT> > alternatives;
		va_list args;
	    va_start(args, fmt);
		    while (*fmt != '\0') 
		    {
		        if (*fmt == 'a') 
		        {
		        	boost::shared_ptr<ALT>* alternative = va_arg(args, boost::shared_ptr<ALT>*);
		            alternatives.push_back(*alternative); 
		        }
		        ++fmt;
		    }
	    va_end(args);

		Expr_Case exprCase(condExpr, alternatives, type);
		boost::variant< Expr_Case > val(exprCase);
		boost::shared_ptr<AST::EXPR> expr(new EXPR(EXPR_CASE, val));

		return expr;
	}

	/**
  	* @param fmt
  				'c' for constructor
  				't' for type
  	*/
	inline boost::shared_ptr<AST::EXPR> getTypedefExpr(std::string typeID, const char *fmt, ...)
	{
		std::vector< boost::shared_ptr<AST::CONSTR> > constructors;

		va_list args;
	    va_start(args, fmt);
		    while (*fmt != '\0') 
		    {
		        if (*fmt == 'c') // read constructor
		        {
		        	char* cnstrID = va_arg(args, char*);
		        	std::string constructorID = std::string(cnstrID);
		        	++fmt;
		        	std::vector< boost::shared_ptr<AST::TYPE> > types;
		        	while (*fmt == 't') // read types associated with constructor
		        	{
		        		boost::shared_ptr<AST::TYPE>* type = va_arg(args, boost::shared_ptr<AST::TYPE>*);
		        		types.push_back(*type);
		        		++fmt;
		        	}
		        	boost::shared_ptr<AST::CONSTR> constructor(new CONSTR(constructorID, types));
		        	constructors.push_back(constructor);
		        }
		    }
	    va_end(args);
		Expr_Typedef typedefinition(typeID, constructors); // i.e. type Time = Hour int | Min int
		boost::variant< Expr_Typedef > value_typedef(typedefinition);
		boost::shared_ptr<AST::EXPR> typeDefExpr(new EXPR(EXPR_TYPE_DEF, value_typedef));

		return typeDefExpr;
	}

	/**
  	* @param fmt
  				'e' for expression
  	*/
	inline boost::shared_ptr<AST::EXPR> getNewVarExpr(std::string ID, std::string typeID, std::string constructorID, const char *fmt, ...)
	{
		std::vector< boost::shared_ptr<AST::EXPR> > params;

		va_list args;
	    va_start(args, fmt);
		    while (*fmt != '\0') 
		    {
		        if (*fmt == 'e') 
		        {
		        	boost::shared_ptr<AST::EXPR>* expr = va_arg(args, boost::shared_ptr<AST::EXPR>*);
		            params.push_back(*expr); 
		        }
		        ++fmt;
		    }
	    va_end(args);

		Expr_Var_Constr constr(constructorID, params);
		boost::variant< Expr_Var_Constr > value_constr(constr);
		boost::shared_ptr<AST::EXPR> constrExpr(new EXPR(EXPR_VAR_CONSTR, value_constr));

		Expr_New_Var constrNewVar(ID, typeID, constrExpr);
		boost::variant< Expr_New_Var > value_constrVar(constrNewVar);
		boost::shared_ptr<AST::EXPR> constrVarExpr(new EXPR(EXPR_NEW_VAR, value_constrVar));

		return constrVarExpr;
	}

	inline boost::shared_ptr<AST::EXPR> getGroupExpr(const char *fmt, ...)
	{
		std::vector< boost::shared_ptr<AST::EXPR> > expressions;

		va_list args;
	    va_start(args, fmt);
		    while (*fmt != '\0') 
		    {
		        if (*fmt == 'e') 
		        {
		        	boost::shared_ptr<AST::EXPR>* expr = va_arg(args, boost::shared_ptr<AST::EXPR>*);
		            expressions.push_back(*expr); 
		        }
		        ++fmt;
		    }
	    va_end(args);

	    Expr_Group exprGroup(expressions);
		boost::variant< Expr_Group > value_group(exprGroup);
		boost::shared_ptr<AST::EXPR> groupExpr(new EXPR(EXPR_GROUP, value_group));

		return groupExpr;
	}
	
	inline boost::shared_ptr<AST::EXPR> getAssignmentExpr(std::string ID, boost::shared_ptr<AST::EXPR> expr)
	{
		std::vector< boost::shared_ptr<AST::EXPR> > values;
		values.push_back(expr);
		Expr_Var_Constr var(ID, values);
		boost::variant< Expr_Var_Constr > value(var);
		boost::shared_ptr<AST::EXPR> assignmentExpr(new EXPR(EXPR_VAR_CONSTR, value));
		
		return assignmentExpr;
	}

protected:
	/*
	* Values
	*/
	boost::variant< bool > value_TRUE;
	boost::variant< bool > value_FALSE;

	boost::variant< int > value_ZERO;
	boost::variant< int > value_ONE;
	boost::variant< int > value_TWO;
	boost::variant< int > value_THREE;
	boost::variant< int > value_21;

	boost::variant< std::string > value_STRING;
	boost::variant< std::string > value_x_STR;

	/*
	* Operators
	*/
	boost::shared_ptr<OPERATOR> op_add;
	boost::shared_ptr<OPERATOR> op_sub;
	boost::shared_ptr<OPERATOR> op_mul;
	boost::shared_ptr<OPERATOR> op_div;
	boost::shared_ptr<OPERATOR> op_eq;
	boost::shared_ptr<OPERATOR> op_less_than;
	boost::shared_ptr<OPERATOR> op_or;
	boost::shared_ptr<OPERATOR> op_and;
	boost::shared_ptr<OPERATOR> op_range;

	/*
	* Expressions
	*/
	boost::shared_ptr<EXPR> expr_TRUE;
	boost::shared_ptr<EXPR> expr_FALSE;

	boost::shared_ptr<EXPR> expr_ZERO;
	boost::shared_ptr<EXPR> expr_ONE;
	boost::shared_ptr<EXPR> expr_TWO;
	boost::shared_ptr<EXPR> expr_THREE;
	boost::shared_ptr<EXPR> expr_21;

	boost::shared_ptr<EXPR> expr_STR;
	boost::shared_ptr<EXPR> expr_x_STR;

	/*
	* Types
	*/
	boost::shared_ptr<TYPE> type_INT;
	boost::shared_ptr<TYPE> type_BOOL; 
	boost::shared_ptr<TYPE> type_STRING;

	boost::shared_ptr<TYPE> type_ZERO;
	boost::shared_ptr<TYPE> type_ONE; 

	/*
	* Alternatives
	*/
	boost::shared_ptr<ALT> alt_ZERO;
	boost::shared_ptr<ALT> alt_ONE; 

	std::vector< boost::shared_ptr<TYPE> > noTypes;
};