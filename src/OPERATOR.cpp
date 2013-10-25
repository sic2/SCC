#include "OPERATOR.h"

#include "EXPR.h"

#define INTEGER_PARAM "I"
#define BOOL_PARAM "Z"

#define ADD_SUBROUTINE "ADD_SUBROUTINE";
#define SUB_SUBROUTINE "SUB_SUBROUTINE";
#define MUL_SUBROUTINE "MUL_SUBROUTINE";
#define DIV_SUBROUTINE "DIV_SUBROUTINE";
#define LESS_THAN_SUBROUTINE "LESS_THAN_SUBROUTINE";
#define EQ_TO_SUBROUTINE "EQ_TO_SUBROUTINE";

AST::OPERATOR::OPERATOR(AST::OP op)
{
	this->_op = op;
}

AST::EXPRESSION_TYPE AST::OPERATOR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, 
	std::string& mainMethod, AST::EXPRESSION_TYPE op0Type, AST::EXPRESSION_TYPE op1Type)
{
	EXPRESSION_TYPE retval = EXPR_UNDEFINED;
	std::string callerByteCode = std::string(PROGRAM_NAME) + ".";
	std::string calleeByteCode;

	std::string returnValue;
	std::string subroutineName;
	bool isSubroutineNeeded = false;
	switch(this->_op)
	{
		case AST::OP_ADDITION:
			{
				subroutineName += ADD_SUBROUTINE;
				returnValue = INTEGER_PARAM;
				retval = EXPR_INT;
			}
			break;
		case AST::OP_SUBTRACTION:
			{
				subroutineName += SUB_SUBROUTINE;
				returnValue = INTEGER_PARAM;
				retval = EXPR_INT;
			}
			break;
		case AST::OP_MULTIPLICATION:
			{
				subroutineName += MUL_SUBROUTINE;
				returnValue = INTEGER_PARAM;
				retval = EXPR_INT;
			}
			break;
		case AST::OP_DIVISION:
			{
				subroutineName += DIV_SUBROUTINE;
				returnValue = INTEGER_PARAM;
				retval = EXPR_INT;
			}
			break;
		case AST::OP_EQUALITY:
			{
				subroutineName += EQ_TO_SUBROUTINE;
				returnValue = BOOL_PARAM;
				retval = EXPR_BOOL;
			}
			break;
		case AST::OP_LESS:
			{
				subroutineName += LESS_THAN_SUBROUTINE;
				returnValue = BOOL_PARAM;
				retval = EXPR_BOOL;
			}
			break;
		case AST::OP_OR:
		case AST::OP_AND:
		case AST::OP_RANGE:
		default:
			printf("operation not supported\n");
		break;
	} // end switch

	// Check operands for parameters
	callerByteCode += subroutineName + getSubroutineParams(op0Type, op1Type) + returnValue;
	calleeByteCode += subroutineName + getSubroutineParams(op0Type, op1Type) + returnValue;

	isSubroutineNeeded = bytecodeGenerator->addSubroutine(callerByteCode); // FIXME - not implemented in bytecodegenerato
	if (isSubroutineNeeded)
	{
		jasminProgram += getSubroutine(calleeByteCode);
	}

	mainMethod += std::string("\tinvokestatic ") + callerByteCode + std::string("\n");
	return retval;
}

std::string AST::OPERATOR::getSubroutineParams(AST::EXPRESSION_TYPE op0Type, AST::EXPRESSION_TYPE op1Type)
{
	std::string retval = "(";
	if (op0Type == EXPR_INT)
	{
		retval += INTEGER_PARAM;
	}
	else if (op0Type == EXPR_BOOL)
	{
		retval += BOOL_PARAM;
	}
	else
	{
		printf("Operator type not supported yet\n");
	}

	if (op1Type == EXPR_INT)
	{
		retval += INTEGER_PARAM;
	}
	else if (op1Type == EXPR_BOOL)
	{
		retval += BOOL_PARAM;
	}
	else
	{
		printf("Operator type not supported yet\n");
	}
	retval += ")";
	return retval;
}

std::string AST::OPERATOR::getSubroutine(std::string& subroutineCallerName)
{
	std::string subroutine = getSubroutineInitialisation(subroutineCallerName);

	switch(this->_op)
	{
		case AST::OP_ADDITION:
		case AST::OP_SUBTRACTION:
		case AST::OP_MULTIPLICATION:
		case AST::OP_DIVISION:
			{
				subroutine += getTrivialMathSubroutine();
			}
			break;
		case AST::OP_EQUALITY:
		case AST::OP_LESS:
			{
				subroutine += getTrivialCmpSubroutine();
			}
			break;
		case AST::OP_OR:
		case AST::OP_AND:
		case AST::OP_RANGE:
		default:
			printf("subroutine not supported yet\n");
		break;
	} // end switch

	subroutine += getSubroutineReturning();
	return subroutine;
}
std::string AST::OPERATOR::getSubroutineInitialisation(std::string& subroutineCallerName)
{
	std::string retval = std::string(".method public static ") + subroutineCallerName + std::string(" ; START SUBROUTINE\n");
	retval += ".limit stack 2 \n";
	retval += ".limit locals 2 \n";
	retval += "\t iload_0 \n";
	retval += "\t iload_1 \n";
	return retval;
}

std::string AST::OPERATOR::getSubroutineReturning()
{
	std::string retval = "\t ireturn \n";
	retval += ".end method ; END SUBROUTINE\n\n";
	return retval;
}

std::string AST::OPERATOR::getTrivialMathSubroutine()
{
	std::string retval;
	switch(this->_op)
	{
		case AST::OP_ADDITION:
			retval = "\t iadd";
			break;
		case AST::OP_SUBTRACTION:
			retval = "\t isub";
			break;
		case AST::OP_MULTIPLICATION:
			retval = "\t imul";
			break;
		case AST::OP_DIVISION:
			retval = "\t idiv";
			break;
		case AST::OP_EQUALITY:
		case AST::OP_LESS:
		case AST::OP_OR:
		case AST::OP_AND:
		case AST::OP_RANGE:
		default:
			printf("Error: This is not a math operation\n");
			break;
	}
	return retval + "\n";
}

std::string AST::OPERATOR::getTrivialCmpSubroutine()
{
	std::string retval;
	if (this->_op == AST::OP_EQUALITY)
	{
		retval += "\t if_icmpne Label_NT \n";
	} 
	else 
	{
	  	retval += "\t if_icmpge Label_NT \n";
	}

	retval += "\t iconst_1 \n";
	retval += "\t goto Label_T \n";
	retval += " Label_NT:\n";
	retval += "\t iconst_0 \n";
	retval += " Label_T: \n";
	return retval;
}
