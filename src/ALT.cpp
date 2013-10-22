#include "AST.h"

AST::ALT::ALT(AST::TYPE* type, AST::EXPR* expr)
{
	_type = type;

	_expr = expr;
	printf("create EXPR %p \n", _expr);
}

AST::ALT::~ALT()
{
	if(DEBUG_MODE >= 2)
	{
		printf("Delete ALT\n");
	}
	printf("delete EXPR %p \n", _expr);
	delete _type;
	delete _expr;
} 