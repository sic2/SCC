#include "AST.h"

AST::PROGRAM::PROGRAM(boost::shared_ptr<AST::EXPR>* expr) 
{
	if(DEBUG_MODE >= 2)
	{
		printf("Creating new Program\n");
	}
	
	_expr = expr;
	printf("start: expr %p from program\n", _expr);
}

void AST::PROGRAM::generateByteCode(std::string& jasminProgram, std::string& mainMethod)
{
	printf("expr %p from program\n", _expr);
	(*_expr)->generateByteCode(jasminProgram, mainMethod);
}