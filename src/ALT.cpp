#include "ALT.h"

#include "EXPR.h"

AST::ALT::ALT(boost::shared_ptr<AST::TYPE> condition, boost::shared_ptr<AST::EXPR> expr)
{
	_condition = condition;
	_expr = expr;
}

boost::shared_ptr<AST::TYPE> AST::ALT::getCondition() 
{ 
	return this->_condition; 
}
	
boost::shared_ptr<AST::EXPR> AST::ALT::getEXPR() 
{ 
	return this->_expr; 
}
