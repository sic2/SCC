#include "OPERATOR.h"
#include "EXPR.h"
#include "ALT.h"
#include "TYPE.h"

#include "JVMByteCodeGenerator.h"

#include <sstream>
#include <map>

#define GET_Condition_FROM_ALT(iterator) (*((* iterator)->getCondition()))
#define GET_EXPR_FROM_ALT(iterator) (*((* iterator)->getEXPR()))

AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value) 
{
	_typeExpr = typeExpr;
	_uValue = value;
}

// Destructor
AST::EXPR::~EXPR() 
{	
	if(DEBUG_MODE >= 2)
	{
		printf("Removing expression of typeExpr %d \n", _typeExpr);
	}
}

AST::EXPRESSION_TYPE AST::EXPR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack)
{
	EXPRESSION_TYPE retval = EXPR_UNDEFINED;
	switch(_typeExpr)
	{
	case EXPR_INT:
		{
			retval = generateIntByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
		}
		break;
	case EXPR_BOOL:
		{
			retval = generateBoolByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
		}
		break;
	case EXPR_STRING:
		{
			// TODO
		}
		break;
	case EXPR_VAR_CONSTR:
		{
			// TODO
			// [ DESIGN ASSUMPTION ]
			// this will work only for primitive types
		}
		break;
	case EXPR_CASE:
		{
			retval = generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
		}
		break;
	case EXPR_FOR_LOOP:
		break;
	case EXPR_BI_OP:
		{
			retval = generateBiOPByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
		}
		break;
	case EXPR_GROUP:
		{
			// TODO
			// Iterate over all expressions
		}
		break;
	case EXPR_TYPE_DEF:
		{
			// TODO
			// Define a new class
		}
		break;
	case EXPR_NEW_VAR:
		{
			// TODO
			// create instance of given class
			//retval = generateNewVarByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
		}
		break;
	default:
		if(DEBUG_MODE >= 1)
		{
	  		printf("Error on getting Jasmin Bytecode from an EXPR\n");
	  	}
	  break;
	} // end switch

	return retval;
}

/******************
* Private methods
*******************/

std::string AST::EXPR::getIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, int Integer)
{
	std::string retval = "iconst_";
	if (Integer == -1)
	{
		return "iconst_m1";
	}
	else if (Integer < -1 || Integer > 5)
	{
		retval = "bipush ";
	}

	return retval + integerToString(Integer);
}

AST::EXPRESSION_TYPE AST::EXPR::generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack)
{	
	std::string bytecode = getIntByteCode(bytecodeGenerator, boost::get< int >(_uValue));
	bytecodeGenerator->formatJasminInstruction(bytecode);
	mainMethod += bytecode;

	std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
	mainMethod += storeBytecode + "\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, onStack);
	return EXPR_INT;
}

AST::EXPRESSION_TYPE AST::EXPR::generateBoolByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack)
{
	if (boost::get< bool >(_uValue) == true)
	{
		mainMethod += "\ticonst_1\n";
	} 
	else
	{
		mainMethod += "\ticonst_0\n";
	}

	std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
	mainMethod += storeBytecode + "\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_BOOL, onStack);

	return EXPR_BOOL;
}

AST::EXPRESSION_TYPE AST::EXPR::generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack) 
{

	boost::shared_ptr<EXPR> expr = boost::get< Expr_Case >(_uValue).expr;
	expr->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true); 	
 	int caseValue = bytecodeGenerator->getEnvironmentSize(); 

 	EXPRESSION_TYPE caseExpr;
	std::vector< boost::shared_ptr<ALT> > alternatives = boost::get< Expr_Case >(_uValue).alternatives;
	for(std::vector< boost::shared_ptr<AST::ALT> >::iterator it = alternatives.begin(); it != alternatives.end(); ++it) 
	{
		if (it != alternatives.begin())
		{
			mainMethod += "Label_" + integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");
		}

		EXPRESSION_TYPE conditionType = GET_Condition_FROM_ALT(it)->getExprType(); 
		if (conditionType == EXPR_INT)
		{		
			mainMethod += std::string("\tiload_") + integerToString(caseValue - 1) + std::string("\n"); // FIXME - assume integer
			GET_Condition_FROM_ALT(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);
			mainMethod += std::string("\tiload_") + integerToString(caseValue) + std::string("\n"); // FIXME - assume integer

			std::string label = "Label_" + integerToString(bytecodeGenerator->nextLabel());
			mainMethod += std::string("\t if_icmpne ") + label + std::string("\n");
			caseExpr = GET_EXPR_FROM_ALT(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);
		}
	} // end for-loop for _alternatives vector
	mainMethod += "Label_" + integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, caseExpr, true);
	return EXPR_CASE;
}

// Generate appropriate subroutine if not there already
// then make a call to this subroutine
// and get the result
// two variables
AST::EXPRESSION_TYPE AST::EXPR::generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack)
{
	int caseValue = bytecodeGenerator->getEnvironmentSize();
	// Generate bytecode for operands

	boost::shared_ptr<EXPR> operand_0 = boost::get< Expr_Bi_Op >(_uValue).expr;
	EXPRESSION_TYPE op0Type = operand_0->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true); // Push to stack, so that next operand can be pushed too
	mainMethod += std::string("\tiload_") + integerToString(caseValue) + std::string("\n"); // FIXME - assume integer

	boost::shared_ptr<EXPR> operand_1 = boost::get< Expr_Bi_Op >(_uValue).expr1;
	EXPRESSION_TYPE op1Type = operand_1->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false); // can be overwritten, so do not update environment stack
	mainMethod += std::string("\tiload_") + integerToString(caseValue + 1) + std::string("\n"); // FIXME - assume integer

	EXPRESSION_TYPE exprType =  (boost::get< Expr_Bi_Op >(_uValue).op)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, op0Type, op1Type);
	mainMethod += getIStoreByteCode(bytecodeGenerator) + "\n";

	std::string ID = "SUBROUTINE";
	bytecodeGenerator->updateEnvironment(&ID, exprType, true);
	return exprType; 
}

/*
* REDO
*/
AST::EXPRESSION_TYPE AST::EXPR::generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack)
{	
	// TODO
	return EXPR_NEW_VAR; // Does this matter?
}

/*
* Utility functions
*/
std::string AST::EXPR::getIStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator)
{
	return "\tistore_" + integerToString(bytecodeGenerator->getEnvironmentSize());
}

std::string AST::EXPR::integerToString(int value)
{
	std::ostringstream convert; 
	convert << value;
	return convert.str();
}