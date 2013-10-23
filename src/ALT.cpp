#include "ALT.h"

#include "EXPR.h"

AST::ALT::ALT(boost::shared_ptr<AST::EXPR>* condition, boost::shared_ptr<AST::EXPR>* expr)
{
	if ((*condition)->getExprType() != EXPR_INT) // TODO - support bools and strings
	{
		printf("Alternative is wrong\n");
	}

	_condition = condition;
	_expr = expr;
}

boost::shared_ptr<AST::EXPR>* AST::ALT::getCondition() 
{ 
	return this->_condition; 
}
	
boost::shared_ptr<AST::EXPR>* AST::ALT::getEXPR() 
{ 
	return this->_expr; 
}
