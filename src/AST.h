#pragma once 

#include <string>
#include <vector>
#include <stdio.h>

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

// AST - Abstrace Syntax Tree
namespace AST 
{
	/*
	* Operands
	*/
	typedef enum OP
	{
		OP_ADDITION,
		OP_SUBTRACTION,
		OP_MULTIPLICATION,
		OP_DIVISION,
		OP_EQUALITY,
		OP_LESS,
		OP_OR,
		OP_AND,
		OP_RANGE
	} OP;

	/*
	 * Types
	 */
	typedef enum PRIMITIVE_TYPE
	{
		TYPE_INT,
		TYPE_BOOL,
		TYPE_STRING
	} PRIMITIVE_TYPE;

	
	/***********************
	***** CLASSES DEFS *****
	** FW - DECLARATIONS ***
	************************/

	/**
	* Operator
	*/
	class OPERATOR;

	/**
	* Type
	*/
	class TYPE;

	/**
	* Expression
	*/
	class EXPR;

	/**
	* Alternative
	*/
	class ALT;
	
	/**
	* Program
	* This class defines the root of the AST.
	* Each program defined in the Case language 
	* consists of one #AST::PROGRAM .
	* Each PROGRAM consists of one and only one #AST::EXPR
	*/
	class PROGRAM;

	/*
	* Expression Types
	*/
	typedef enum EXPRESSION_TYPE
	{
		EXPR_INT,
		EXPR_BOOL,
		EXPR_STRING,
		EXPR_VAR_CONSTR,
		EXPR_CASE,
		EXPR_FOR_LOOP,
		EXPR_BI_OP,
		EXPR_GROUP,
		EXPR_TYPE_DEF,
		EXPR_NEW_VAR,
		EXPR_UNDEFINED
	} EXPRESSION_TYPE;

	/*******************************************************
	* The following classes contain public fields on purpose
	* since the aim is to use these more like structs 
	* even if in C++ structs == classes
	*
	* TODO
	* possible to use references? 
	*******************************************************/
	class Expr_Var_Constr{
	public:
		Expr_Var_Constr(std::string ID, boost::shared_ptr<EXPR> expr)
		{
			this->ID = ID;
			this->expr = expr;
		}

		virtual ~Expr_Var_Constr() {}
		std::string ID;
		boost::shared_ptr<EXPR> expr;
	};

	class Expr_Case{
	public:
		Expr_Case(boost::shared_ptr<EXPR> expr, std::vector< boost::shared_ptr<ALT> > alternatives)
		{
			this->expr = expr;
			this->alternatives = alternatives;
		}

		virtual ~Expr_Case() {}
		boost::shared_ptr<EXPR> expr;
		std::vector< boost::shared_ptr<ALT> > alternatives;
	};

	class Expr_For_Loop{
	public:
		Expr_For_Loop(std::string ID, boost::shared_ptr<EXPR> expr, boost::shared_ptr<EXPR> expr1)
		{
			this->ID = ID;
			this->expr = expr;
			this->expr1 = expr1;
		}

		virtual ~Expr_For_Loop() {}
		std::string ID;
		boost::shared_ptr<EXPR> expr;
		boost::shared_ptr<EXPR> expr1;
	};

	class Expr_Bi_Op{
	public:
		Expr_Bi_Op(boost::shared_ptr<EXPR> expr, boost::shared_ptr<OPERATOR> op, boost::shared_ptr<EXPR> expr1)
		{
			this->expr = expr;
			this->op = op;
			this->expr1 = expr1;
		}

		virtual ~Expr_Bi_Op() {}
		boost::shared_ptr<EXPR> expr;
		boost::shared_ptr<OPERATOR> op;
		boost::shared_ptr<EXPR> expr1;
	};
	
	class Expr_Group{
	public:
		Expr_Group(std::vector< boost::shared_ptr<EXPR> > expressions)
		{
			this->expressions = expressions;
		}

		virtual ~Expr_Group() {}
		std::vector< boost::shared_ptr<EXPR> > expressions;
	};

	class Expr_New_Var{
	public:
		Expr_New_Var(std::string ID, boost::shared_ptr<TYPE> type, boost::shared_ptr<EXPR> expr)
		{
			this->ID = ID;
			this->type = type;
			this->expr = expr;
		}

		virtual ~Expr_New_Var() {}
		std::string ID;
		boost::shared_ptr<TYPE> type;
		boost::shared_ptr<EXPR> expr;
	};

	typedef boost::variant< 
							int, 
							bool,
							std::string,
							Expr_Var_Constr, 
							Expr_Case, 
							Expr_For_Loop, 
							Expr_Bi_Op, 
							Expr_Group,
							Expr_New_Var
						> uValue; 
}