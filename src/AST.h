#pragma once 

#include <string>
#include <vector>
#include <stdio.h>

#include <boost/shared_ptr.hpp>

// http://stackoverflow.com/questions/18188612/abstract-syntax-tree-representation-in-c
// http://stackoverflow.com/questions/16127385/recursive-descent-parser-example-for-c

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

	/*********************
	***** UNIONS *********
	**********************/

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

	// EXPR_VAR_CONSTR
	typedef struct Expr_Var_Constr 
	{
		std::string* ID;
		boost::shared_ptr<EXPR>* expr;
	} Expr_Var_Constr;

	// EXPR_CASE
	typedef struct Expr_Case 
	{
		 boost::shared_ptr<EXPR>* expr;
		 std::vector< boost::shared_ptr<ALT> >* alternatives;
	} Expr_Case;

	// EXPR_FOR_LOOP
	typedef struct Expr_For_Loop 
	{
		std::string* ID;
		boost::shared_ptr<EXPR>* expr;
		boost::shared_ptr<EXPR>* expr1;
	} Expr_For_Loop;

	// EXPR_BI_OP
	typedef struct Expr_Bi_Op 
	{
		boost::shared_ptr<EXPR>* expr;
		boost::shared_ptr<OPERATOR>* op;
		boost::shared_ptr<EXPR>* expr1;
	} Expr_Bi_Op;

	// EXPR_GROUP
	typedef struct Expr_Group 
	{
		std::vector< boost::shared_ptr<EXPR> >* expressions;
	} Expr_Group;

	// EXPR_NEW_VAR
	typedef struct Expr_New_Var 
	{
		std::string* ID;
		boost::shared_ptr<EXPR>* expr;
	} Expr_New_Var;

	/*
	* Expressions
	*/ 
	typedef union uValue
	{
		int Integer; 
		bool Bool; 
		std::string* Str; 
		Expr_Var_Constr exprVarConstr; 
		Expr_Case exprCase; 
		Expr_For_Loop exprForLoop; 
		Expr_Bi_Op exprBiOp; 
		Expr_Group exprGroup; 
		Expr_New_Var exprNewVar;
	} uValue;
}