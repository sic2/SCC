#include "OPERATOR.h"
#include "EXPR.h"
#include "ALT.h"
#include "TYPE.h"

#include "JVMByteCodeGenerator.h"

#include <sstream>
#include <map>

#define GET_Condition_FROM_ALT(iterator) (*((* iterator)->getCondition()))
#define GET_EXPR_FROM_PAIR(iterator) (*(* iterator).first->getEXPR())


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

AST::EXPRESSION_TYPE AST::EXPR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
{
	EXPRESSION_TYPE retval = EXPR_UNDEFINED;
	switch(_typeExpr)
	{
	case EXPR_INT:
		{
			retval = generateIntByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
		}
		break;
	case EXPR_BOOL:
		{
			// TODO
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
		}
		break;
	case EXPR_CASE:
		{
			retval = generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
		}
		break;
	case EXPR_FOR_LOOP:
		break;
	case EXPR_BI_OP:
		{
			retval = generateBiOPByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
		}
		break;
	case EXPR_GROUP:
		{
			// TODO
		}
		break;
		
	case EXPR_TYPE_DEF:
		{
			// TODO
		}
		break;
	case EXPR_NEW_VAR:
		{
			retval = generateNewVarByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
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

AST::EXPRESSION_TYPE AST::EXPR::generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
{	
	if (printStream)
	{
		mainMethod += std::string("\t") + PRINT_STREAM + std::string("\n");
	}
	
	std::string bytecode = getIntByteCode(bytecodeGenerator, _uValue.Integer);
	bytecodeGenerator->formatJasminInstruction(bytecode);
	mainMethod += bytecode;

	std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
	mainMethod += storeBytecode + "\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, false);

	return EXPR_INT;
}

// [ ASSUMPTION ] - case of integers 
// TODO - case for other types !? 
AST::EXPRESSION_TYPE AST::EXPR::generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream) 
{
	boost::shared_ptr<EXPR>* expr = _uValue.exprCase.expr;
	(*expr)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true);
 
 	mainMethod += std::string("\tiload_") + integerToString(bytecodeGenerator->getEnvironmentSize()) + std::string("\n");
	mainMethod += "\tlookupswitch \n";

	// A vector is created to store each ALTernative, its own label as well
	std::vector< std::pair< boost::shared_ptr<AST::ALT>, std::string> > pairsAltsLabels;  // FIXME - do not limit this to integers
	int labelIndex = 0;
	
	/*
	* Loop through all alternatives and print the first part of the lookupswitch
	* example:
	* 1 : Label_0
	* 2 : Label_1
	* 324 : Label_2
	* etc..
	*/
	std::vector< boost::shared_ptr<ALT> >* alternatives = _uValue.exprCase.alternatives;
	for(std::vector< boost::shared_ptr<AST::ALT> >::iterator it = alternatives->begin(); it != alternatives->end(); ++it) 
	{
		EXPRESSION_TYPE expressionType;
		uValue value;
		
		expressionType = GET_Condition_FROM_ALT(it)->getExprType(); 
		value = GET_Condition_FROM_ALT(it)->getValue();
		
		if (expressionType == EXPR_INT)
		{
			/*
			* example:
			* 	0 : Label_0
			* OR
			* 	1 : Label_1
			* etc..
			*/
			mainMethod += "\t\t" + integerToString(value.Integer);
			std::string label = "Label_" + integerToString(labelIndex);
			mainMethod += " : " + label + "\n";
			labelIndex++;

			// Adding the integer and the label to pairsAltsLabels
			pairsAltsLabels.push_back(std::make_pair< boost::shared_ptr<AST::ALT>, std::string>((*it), label));
		}
		
	} // end for-loop for _alternatives vector
	
	// Default label must always be specified - but it won't have
	// any functionality in the Case language
	mainMethod += "\t\tdefault : DLABEL\n"; 

	/* Loop pairsAltsLabels to expand each case 
	* example:
	* Label_0:
		EXPR
	* Label_1:
		EXPR
	* etc...
	*/
	for(std::vector< std::pair< boost::shared_ptr<AST::ALT>, std::string> >::iterator it = pairsAltsLabels.begin(); 
		it != pairsAltsLabels.end(); ++it) 
	{
		mainMethod += (*it).second + ": \n";
		GET_EXPR_FROM_PAIR(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);
		mainMethod += "\tgoto DLABEL\n";
	} // end for-loop for pairsAltsLabels vector
	
	// Handle default label
	mainMethod += "DLABEL: \n"; 

	return EXPR_CASE;
}

// Generate appropriate subroutine if not there already
// then make a call to this subroutine
// and get the result
// two variables
AST::EXPRESSION_TYPE AST::EXPR::generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
{
	// Generate bytecode for operands
	boost::shared_ptr<EXPR>* operand_0 = _uValue.exprBiOp.expr;
	EXPRESSION_TYPE op0Type = (*operand_0)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true);

	boost::shared_ptr<EXPR>* operand_1 = _uValue.exprBiOp.expr1;
	EXPRESSION_TYPE op1Type = (*operand_1)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);

	EXPRESSION_TYPE exprType =  (*_uValue.exprBiOp.op)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, op0Type, op1Type);
	mainMethod += getIStoreByteCode(bytecodeGenerator) + "\n";

	std::string ID = "SUBROUTINE";
	bytecodeGenerator->updateEnvironment(&ID, exprType, true);
	return exprType; 
}

AST::EXPRESSION_TYPE AST::EXPR::generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
{	
	std::string* ID = _uValue.exprNewVar.ID;
	boost::shared_ptr<EXPR>* expr = _uValue.exprNewVar.expr;

	if ((*expr)->getExprType() == EXPR_INT)
	{
		int value = (*expr)->getValue().Integer;
		mainMethod += getIntByteCode(bytecodeGenerator, value) + std::string("\t; Var: ") + *ID + std::string("\n");

		std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
		mainMethod += storeBytecode + "\n";

		bytecodeGenerator->updateEnvironment(ID, EXPR_INT, true);
	}

	return (*expr)->getExprType();
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