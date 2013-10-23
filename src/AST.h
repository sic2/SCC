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
		EXPR_NEW_VAR
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
		OP op;
		boost::shared_ptr<EXPR>* expr1;
	} Expr_Bi_Op;

	// EXPR_GROUP
	typedef struct Expr_Group 
	{
		std::vector< boost::shared_ptr<EXPR> >* expressions;
	} Expr_Group;

	/*
	* Expressions
	*/ 
	typedef union uValue
	{
		int Integer; // int value
		bool Bool; // bool value
		std::string* Str; // string value
		Expr_Var_Constr exprVarConstr; 
		Expr_Case exprCase; 
		Expr_For_Loop exprForLoop; 
		Expr_Bi_Op exprBiOp; 
		Expr_Group exprGroup; 
	} uValue;
}

/*******************
** CLASSES DEFs ****
*******************/

class AST::TYPE
{
public:
	TYPE(PRIMITIVE_TYPE primitiveType, uValue value);

  	TYPE(std::string ID, TYPE type);

  	TYPE(std::string ID, std::vector<TYPE> types);

  	/**
  	* @param [out] primitiveType
  	* @param [out] value
  	* @return false if the type is not an int, bool or string
  	*/
  	bool getValue(PRIMITIVE_TYPE* primitiveType, uValue* value);

private:
	PRIMITIVE_TYPE _primitiveType;
	uValue _uValue;
	std::string _id;
	std::vector<TYPE> _types;

	int _whichConstructor;
};

class AST::EXPR
{
public:

	EXPR(EXPRESSION_TYPE typeExpr, uValue value);
	virtual ~EXPR();

	/*
	* Other methods
	*/ 
	void generateByteCode(std::string& jasminProgram, std::string& mainMethod);

private:
	EXPRESSION_TYPE _typeExpr;
	uValue _uValue;

	std::string getIntByteCode(int Integer);
	void generateCaseByteCode(std::string& jasminProgram, std::string& mainMethod);
	void generateBiOPByteCode(std::string& jasminProgram, std::string& mainMethod);
};

class AST::ALT
{
public:
	ALT(boost::shared_ptr<TYPE>* type, boost::shared_ptr<EXPR>* expr);
	virtual ~ALT() {}

	boost::shared_ptr<TYPE>* getTYPE(); 
	boost::shared_ptr<EXPR>* getEXPR();

private:
	boost::shared_ptr<TYPE>* _type;
	boost::shared_ptr<EXPR>* _expr;
};

class AST::PROGRAM 
{
public:
	PROGRAM(boost::shared_ptr<EXPR>* expr);
	virtual ~PROGRAM() {}

	/**
	* Traverse the AST tree and transform it
	* in Jasmin bytecode.
	* @param [out] jasminProgram represent the entire jasmin bytecode program, 
	*				excluding the main method. 
	*				Additional methods should be added to it.
	* @param [out] mainMethod used for the bytecode of the main method of the program.
	* @note The mainMethod should added at the end of the jasminProgram by the caller.
	*/
	void generateByteCode(std::string& jasminProgram, std::string& mainMethod);

private:
	boost::shared_ptr<EXPR>* _expr;
};