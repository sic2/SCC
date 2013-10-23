#pragma once 

#include <string>
#include <vector>
#include <stdio.h>

#include <boost/shared_ptr.hpp>

// http://stackoverflow.com/questions/18188612/abstract-syntax-tree-representation-in-c
// http://stackoverflow.com/questions/16127385/recursive-descent-parser-example-for-c

// AST - Abstrace Syntax Tree
/*
* TODO - docs
*/
namespace AST 
{
	/******************
	**** ENUMS *********
	*******************/

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
	class TYPE;
	class EXPR;
	class ALT;
	class ALTS;
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
		EXPR* expr;
	} Expr_Var_Constr;

	typedef struct Expr_Case 
	{
		 EXPR* expr;
		 std::vector< boost::shared_ptr<ALT> >* alternatives;
	} Expr_Case;

	typedef struct Expr_For_Loop 
	{
		std::string* ID;
		EXPR* expr;
		EXPR* expr1;
	} Expr_For_Loop;

	typedef struct Expr_Bi_Op 
	{
		EXPR* expr;
		OP op;
		EXPR* expr1;
	} Expr_Bi_Op;

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
	void generateByteCode(std::string& output);

private:
	EXPRESSION_TYPE _typeExpr;
	uValue _uValue;

	std::string getIntByteCode(int Integer);
	void generateCaseByteCode(std::string& output);
	void generateBiOPByteCode(std::string& output);
};

/*
* Alternatives
*/
class AST::ALT
{
public:
	ALT(TYPE* type, EXPR* expr);
	virtual ~ALT();

	// TODO - move method implementation to cpp file
	TYPE* getTYPE() { return this->_type; }
	EXPR* getEXPR() { return this->_expr; }

private:
	TYPE* _type;
	EXPR* _expr;
};

class AST::PROGRAM 
{
public:
	inline PROGRAM(EXPR* expr) 
	{
		if(DEBUG_MODE >= 2)
		{
			printf("Creating new Program\n");
		}
		
		_expr = expr;
	}

	virtual ~PROGRAM() 
	{
		if(DEBUG_MODE >= 2)
		{
			printf("Delete Program expression\n\n");
		}
		delete _expr;
	}

	inline void generateByteCode(std::string& output)
	{
		_expr->generateByteCode(output);
	}

private:
	EXPR* _expr;
};