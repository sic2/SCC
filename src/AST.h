#include <string>
#include <vector>
#include <stdio.h>

// http://stackoverflow.com/questions/18188612/abstract-syntax-tree-representation-in-c
// http://stackoverflow.com/questions/16127385/recursive-descent-parser-example-for-c

// AST - Abstrace Syntax Tree
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

	/*********************
	***** UNIONS *********
	**********************/

	/*
	* Expressions
	*/ 
	typedef union uValue
	{
		int Integer; // int value
		bool Bool; // bool value
		std::string* Str; // string value
	} uValue;

	/***********************
	***** CLASSES DEFS *****
	** FW - DECLARATIONS ***
	************************/
	class TYPE;
	class EXPR;
	class ALT;
	class PROGRAM;
}

/*******************
** CLASSES DEFS ****
*******************/

class AST::TYPE
{
public:
	TYPE(PRIMITIVE_TYPE primitiveType);

  	TYPE(std::string ID, TYPE type);

  	TYPE(std::string ID, std::vector<TYPE> types);

private:
  PRIMITIVE_TYPE _primitiveType;
  std::string _id;
  std::vector<TYPE> _types;

  int _whichConstructor;
};

class AST::EXPR
{
public:
	EXPR(EXPRESSION_TYPE typeExpr, uValue value);
	// EXPR_VAR_CONSTR
	EXPR(EXPRESSION_TYPE typeExpr, std::string ID, EXPR* expr);
	// EXPR_CASE
	EXPR(EXPRESSION_TYPE typeExpr, EXPR* expr /* ALTS */);
	// EXPR_FOR_LOOP
	EXPR(EXPRESSION_TYPE typeExpr, std::string ID, EXPR* expr0, EXPR* expr1);
	EXPR(EXPRESSION_TYPE typeExpr, EXPR* expr0, std::string op, EXPR* expr1);

	virtual ~EXPR();

private:
	uValue _uValue;
	EXPR* _expr0;
	EXPR* _expr1;

	EXPRESSION_TYPE _typeExpr;
	OP _operand;
	std::string _id;
};

/*
* Alternatives
*/
class AST::ALT
{
public:
	ALT(TYPE* type, EXPR* expr);
	virtual ~ALT();

private:
	TYPE* _type;
	EXPR* _expr;
};

class AST::PROGRAM 
{
public:
	inline PROGRAM(EXPR* expr) 
	{
		_expr = expr;
	}

	virtual ~PROGRAM() 
	{
		printf("Deleteing main PROGRAM expression\n\n");
		delete _expr;
	}

private:
	EXPR* _expr;
};