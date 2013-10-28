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

AST::EXPRESSION_TYPE AST::PROGRAM::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{
	int dummy;
	return (_expr)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true, &dummy);
}