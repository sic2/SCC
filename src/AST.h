#include <string>
#include <vector>
#include <stdio.h>

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
	  INT,
	  BOOL,
	  STRING
	} PRIMITIVE_TYPE;

	class TYPE
	{
	public:
	  inline TYPE(PRIMITIVE_TYPE primitiveType)
	  {
	    _primitiveType = primitiveType;
	    _whichConstructor = 0;
	  }

	  inline TYPE(char OPEN_BRACKET, std::string ID, TYPE type, char CLOSED_BRACKET)
	  {
	    if(OPEN_BRACKET == '(' && CLOSED_BRACKET == ')')
	      {
		_id = ID;
		_types.push_back(type);
		_whichConstructor = 1;
	      }
	    else 
	      {
		printf("Error on type constructor (1)\n");
	      }
	  } 

	  inline TYPE(char OPEN_BRACKET, std::string ID, std::vector<TYPE> types, char CLOSED_BRACKET)
	  {
	    if (OPEN_BRACKET == '(' && CLOSED_BRACKET == ')')
	      {
		_id = ID;
		_types = types;
		_whichConstructor = 2;
	      }
	    else 
	      {
		printf("Error on type constructor (2)\n");
	      }

	  }

	private:
	  PRIMITIVE_TYPE _primitiveType;
	  std::string _id;
	  std::vector<TYPE> _types;
	  int _whichConstructor;
	  
	};


	typedef enum EXPRESSION_VALUE_TYPE
	{
		EXPR_INT,
		EXPR_BOOL,
		EXPR_STRING
	} EXPRESSION_VALUE_TYPE;

	/*
	* Expressions
	*/ 
	typedef union uValue
	{
		int Integer; // int value
		bool Bool; // bool value
		std::string* Str; // string value
		// TODO Others
	} uValue;

	class EXPR
	{
	public:
		inline EXPR(EXPRESSION_VALUE_TYPE typeExprValue, uValue value) 
		{
			_uValue = value;
			_typeExprValue = typeExprValue;
			_whichConstructor = 0;
		}
		inline EXPR(EXPR* expr0, char op, EXPR* expr1) 
		{
			_expr0 = expr0;
			_expr1 = expr1;
			switch(op) // FIXME - do not use switch, since there are ops with more than one char (i.e. ==, div, ..) - simple solution is using an if/else chain
			{
			case '+':
			  _operand = OP_ADDITION;
			  break;
			case '-':
			  _operand = OP_SUBTRACTION;
			  break;
			case '*':
			  _operand = OP_MULTIPLICATION;
			  break;
			case '/':
			  _operand = OP_DIVISION;
			  break;
			case '=': 
			  _operand = OP_EQUALITY;
			  break;
			case '<':
			  _operand = OP_LESS;
			  break;
			  // TODO - OTHER operands
			default:
			  printf("Operand not supported \n");
			  break;
			} // end switch

			_whichConstructor = 1;
		}
		
		inline EXPR(std::string ID, EXPR* expr)
		{
		        _id = ID;
		        _expr0 = expr;
		        _whichConstructor = 2;
		}

		inline EXPR(std::string CASE, EXPR* expr, std::string OF /* ALTS */)
		{
		  if (CASE.compare("case") == 0 && OF.compare("of") == 0)
		    {
		      _expr0 = expr;
		      // TODO - ALTS
		      _whichConstructor = 3;
		    }
		  else
		    {
		      printf("Case statement not correct\n");
		    }
		}

		inline EXPR(std::string FOR, std::string ID, std::string IN, EXPR* expr0, std::string DO, EXPR* expr1)
		{
		  if (FOR.compare("for") == 0 &&
		      IN.compare("in") == 0 &&
		      DO.compare("do") == 0)
		    {
		      _expr0 = expr0;
		      _expr1 = expr1;
		      _whichConstructor = 4;
		    }
		  else 
		    {
		      printf("For statement not correct\n");
		    }

		}

		virtual ~EXPR() 
		{	
			printf("Removing expression\n");
			switch(_whichConstructor)
			{
			case 1:
			case 4:
			  {
			    delete _expr0;
			    delete _expr1;
			  }
			  break;
			case 2:
			  {
			    delete _expr0;
		          }
		          break;
			case 3:
			  {
			    delete _expr0;
			    // TODO - delete ALTS
			  }
			  break;
			default:
			  printf("Error on sExpressions Destructor\n");
			  break;
			} 
		}

	private:
		uValue _uValue;
		EXPR* _expr0;
		EXPR* _expr1;

		EXPRESSION_VALUE_TYPE _typeExprValue;
		OP _operand;
		std::string _id;

		int _whichConstructor;
	};

	class PROGRAM 
	{
	public:
		inline PROGRAM(EXPR expr, char endExpression) 
		: _expr(expr)
	  {
	    if (endExpression != '.')
	      {
		printf("Program not terminating correctly\n");
	      } 
	  }

	private:
		EXPR& _expr;
	};
}
