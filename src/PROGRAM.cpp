#include "PROGRAM.h"

#include "EXPR.h"

AST::PROGRAM::PROGRAM(boost::shared_ptr<AST::EXPR> expr) 
{
	if(DEBUG_MODE >= 2)
	{
		printf("Creating new Program\n");
	}
	
	_expr = expr;
}

void AST::PROGRAM::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{
	stateContext cntx;
	cntx.forceLoad = false;
	(_expr)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true, &cntx);
}