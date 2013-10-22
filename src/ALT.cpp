#include "AST.h"

AST::ALT::ALT(AST::TYPE* type, AST::EXPR* expr)
{
	_type = type;
	_expr = expr;
}

AST::ALT::~ALT()
{
	if(DEBUG_MODE >= 2)
	{
		printf("Delete ALT\n");
	}
	
	delete _type;
	delete _expr;
} 