#include "AST.h"

AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value) 
{
	_typeExpr = typeExpr;
	_uValue = value;
}

// EXPR_VAR_CONSTR
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, std::string ID, AST::EXPR* expr)
{
	_typeExpr = typeExpr;
    _id = ID;
    _expr0 = expr;
}

// EXPR_CASE
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::EXPR* expr /* ALTS */)
{
	_typeExpr = typeExpr;
	_expr0 = expr;
	// TODO - ALTS
}

// EXPR_FOR_LOOP
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, std::string ID, AST::EXPR* expr0, AST::EXPR* expr1)
{
	_typeExpr = typeExpr;
	_id = ID;
	_expr0 = expr0;
	_expr1 = expr1;
}

AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::EXPR* expr0, std::string op, AST::EXPR* expr1) 
{
	_typeExpr = typeExpr;
	_expr0 = expr0;
	_expr1 = expr1;
	if (op.compare("+") == 0)
	{
		 _operand = OP_ADDITION;
	}
	else if (op.compare("-") == 0)
	{
		_operand = OP_SUBTRACTION;
	}
	else if (op.compare("*") == 0)
	{
		_operand = OP_MULTIPLICATION;
	}
	else if (op.compare("div") == 0)
	{
		_operand = OP_DIVISION;
	}
	else if (op.compare("==") == 0)
	{
		_operand = OP_EQUALITY;
	}
	else if (op.compare("<") == 0)
	{
		_operand = OP_LESS;
	}
	else if (op.compare("or") == 0 || op.compare("||") == 0)
	{
		_operand = OP_OR;
	}
	else if (op.compare("and") == 0 || op.compare("&&") == 0)
	{
		_operand = OP_AND;
	}
	else if (op.compare("..") == 0)
	{
		_operand = OP_RANGE;
	}
	else
	{
		if(DEBUG_MODE >= 1)
		{
			printf("Operand %d Unknown\n", _operand);
		}
	}

}

// Destructor
AST::EXPR::~EXPR() 
{	
	if(DEBUG_MODE >= 2)
	{
		printf("Removing expression of typeExpr %d \n", _typeExpr);
	}

	switch(_typeExpr)
	{
	case EXPR_INT:
	case EXPR_BOOL:
	case EXPR_STRING:
		break;
	case EXPR_VAR_CONSTR:
		{
			delete _expr0;
		}
		break;
	case EXPR_CASE:
		{
			delete _expr0;
		}
		break;
	case EXPR_FOR_LOOP:
	case EXPR_BI_OP:
		{
			delete _expr0;
			delete _expr1;
		}
		break;
	// TODO	
	case EXPR_GROUP:
	case EXPR_TYPE_DEF:
	case EXPR_NEW_VAR:
	default:
		if(DEBUG_MODE >= 1)
		{
	  		printf("Error EXPR Destructor\n");
	  	}
	  break;
	}  // end switch
}