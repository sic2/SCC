#include "AST.h"

AST::ALT::ALT(boost::shared_ptr<AST::TYPE>* type, boost::shared_ptr<AST::EXPR>* expr)
{
	_type = type;
	_expr = expr;
}

boost::shared_ptr<AST::TYPE>* AST::ALT::getTYPE() 
{ 
	return this->_type; 
}
	
boost::shared_ptr<AST::EXPR>* AST::ALT::getEXPR() 
{ 
	return this->_expr; 
}
