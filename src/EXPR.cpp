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

void AST::EXPR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
{
	switch(_typeExpr)
	{
	case EXPR_INT:
		{
			generateIntByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
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
			generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
		}
		break;
	case EXPR_FOR_LOOP:
		break;
	case EXPR_BI_OP:
		{
			generateBiOPByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
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
			generateNewVarByteCode(bytecodeGenerator, jasminProgram, mainMethod, printStream);
		}
		break;
	default:
		if(DEBUG_MODE >= 1)
		{
	  		printf("Error on getting Jasmin Bytecode from an EXPR\n");
	  	}
	  break;
	} // end switch
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

void AST::EXPR::generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
{	
	if (printStream)
	{
		mainMethod += PRINT_STREAM + std::string("\n");
	}
	
	std::string bytecode = getIntByteCode(bytecodeGenerator, _uValue.Integer);
	bytecodeGenerator->formatJasminInstruction(bytecode);
	mainMethod += bytecode;

	std::string ID = ""; // dummy ID
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, false);
}

// [ ASSUMPTION ] - case of integers
void AST::EXPR::generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream) 
{
	boost::shared_ptr<EXPR>* expr = _uValue.exprCase.expr;
	(*expr)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);

	mainMethod += "\tistore_0\n"; // FIXME - hardcoded. 
	mainMethod += "\tiload_0 \n"; // FIXME - hardcoded bytecode
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
		GET_EXPR_FROM_PAIR(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false); // FIXME - not sure about last param
		mainMethod += "\treturn\n";
	} // end for-loop for pairsAltsLabels vector
	
	// Handle default label
	mainMethod += "DLABEL: \n"; 
	mainMethod += "\treturn\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_CASE, false);
}

void AST::EXPR::generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
{
	// TODO
	// Generate appropriate subroutine if not there already
	// then make a call to this subroutine
	// and get the result
	// two variables

	// FIXME - assuming OP is ADDITION
	// FIXME - assuming operands are integers

	// Generate bytecode for operands
	boost::shared_ptr<EXPR>* operand_0 = _uValue.exprBiOp.expr;
	(*operand_0)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true);

	boost::shared_ptr<EXPR>* operand_1 = _uValue.exprBiOp.expr1;
	(*operand_1)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);

	OP op = _uValue.exprBiOp.op;
	switch(op)
	{
		case OP_ADDITION:
			generateAdditionByteCode(bytecodeGenerator, jasminProgram, mainMethod);
			break;
		case OP_SUBTRACTION:
			generateSubtractionByteCode(bytecodeGenerator, jasminProgram, mainMethod);
			break;
		case OP_MULTIPLICATION:
			break;
		case OP_DIVISION:
			break;
		case OP_EQUALITY:
			break;
		case OP_LESS:
			break;
		case OP_OR:
			break;
		case OP_AND:
			break;
		case OP_RANGE:
			break;
		default:
			if(DEBUG_MODE >= 1)
			{
		  		printf("Error on operand: %d\n", op);
		  	}
			break;
	}

}

void AST::EXPR::generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool printStream)
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
}

void AST::EXPR::generateAdditionByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{
	mainMethod += "\tinvokestatic " + std::string(PROGRAM_NAME) + "." + JVMByteCodeGenerator::ADD_SUBROUTINE + "(II)I\n";
	if (!bytecodeGenerator->isAddSubroutineEnabled())
	{
		bytecodeGenerator->addSubroutine(jasminProgram);
	}
	mainMethod += getIStoreByteCode(bytecodeGenerator) + "\n";

	std::string ID = "ADDITION";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, true); // FIXME - assume integer addition
}

void AST::EXPR::generateSubtractionByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{
	mainMethod += "\tinvokestatic " + std::string(PROGRAM_NAME) + "." + JVMByteCodeGenerator::SUB_SUBROUTINE + "(II)I\n";
	if (!bytecodeGenerator->isSubSubroutineEnabled())
	{
		bytecodeGenerator->subSubroutine(jasminProgram);
	}
	mainMethod += getIStoreByteCode(bytecodeGenerator) + "\n";

	std::string ID = "SUBTRACTION";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, true); // FIXME - assume integer addition
}


/*
* Utility functions
*/
std::string AST::EXPR::getIStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator)
{
	return "istore_" + integerToString(bytecodeGenerator->getEnvironmentSize());
}

std::string AST::EXPR::integerToString(int value)
{
	std::ostringstream convert; 
	convert << value;
	return convert.str();
}