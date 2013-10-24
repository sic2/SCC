#include "EXPR.h"
#include "ALT.h"
#include "TYPE.h"

#include "JVMByteCodeGenerator.h"

#include <sstream>
#include <map>

#define INTEGER_BASE 10
#define MAX_NUMBER_DIGITS 32

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
	printf("delete expr with typeExpr %d\n", _typeExpr);
	if(DEBUG_MODE >= 2)
	{
		printf("Removing expression of typeExpr %d \n", _typeExpr);
	}
}

void AST::EXPR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{
	// Update the bytecodeGenerator with the current type of expression.
	bytecodeGenerator->updateLastExpressionType(_typeExpr);

	switch(_typeExpr)
	{
	case EXPR_INT:
		{
			// std::string bytecode = getIntByteCode(bytecodeGenerator, _uValue.Integer);
			// bytecodeGenerator->formatJasminInstruction(bytecode);
			// mainMethod += bytecode;
			// std::string store = "istore_0"; // FIXME - hardcoded bytecode // FIXME - SHOULD NOT STORE!	
			// bytecodeGenerator->formatJasminInstruction(store);
			// mainMethod += store;
			//JVMByteCodeGenerator::printInt(output, 0);
			generateIntByteCode(bytecodeGenerator, jasminProgram, mainMethod);
		}
		break;
	case EXPR_BOOL:
		// TODO
		break;
	case EXPR_STRING:
		// TODO	
		break;
	case EXPR_VAR_CONSTR:
		{
			// printf("destr: this statement should never be reached\n");
		}
		break;
	case EXPR_CASE:
		{
			generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod);
		}
		break;
	case EXPR_FOR_LOOP:
		break;
	case EXPR_BI_OP:
		{
			generateBiOPByteCode(bytecodeGenerator, jasminProgram, mainMethod);
		}
		break;
	// TODO	
	case EXPR_GROUP:
		// TODO
	case EXPR_TYPE_DEF:
		// TODO
	case EXPR_NEW_VAR:
		{
			generateNewVarByteCode(bytecodeGenerator, jasminProgram, mainMethod);
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

/*
* Private methods
*/
void AST::EXPR::generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{
	std::map< std::string, std::pair<int, AST::EXPRESSION_TYPE> > env = bytecodeGenerator->getEnvironment();
	unsigned int environmentSize = env.size();

	std::string bytecode = getIntByteCode(bytecodeGenerator, _uValue.Integer);
	bytecodeGenerator->formatJasminInstruction(bytecode);
	mainMethod += bytecode;

	std::ostringstream convert; 
	convert << environmentSize;
	std::string storeBytecode = "istore_" + convert.str();

	bytecodeGenerator->formatJasminInstruction(storeBytecode);
	mainMethod += storeBytecode;

	std::string ID = ""; // FIXME - dummy ID
	updateEnvironment(bytecodeGenerator, &ID, environmentSize, EXPR_INT);
}

std::string AST::EXPR::getIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, int Integer)
{
	std::ostringstream convert; 
	convert << Integer;

	std::string retval = "iconst_";
	if (Integer == -1)
	{
		return "iconst_m1";
	}
	else if (Integer < -1 || Integer > 5)
	{
		retval = "bipush ";
	}

	return retval + convert.str();
}

// [ ASSUMPTION ] - case of integers
void AST::EXPR::generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod) 
{
	boost::shared_ptr<EXPR>* expr = _uValue.exprCase.expr;
	(*expr)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod);

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
		
		// [ ENORMOUS BULLSHIT ]
		// getValue function does not even exist
		expressionType = GET_Condition_FROM_ALT(it)->getExprType(); 
		value = GET_Condition_FROM_ALT(it)->getValue(); // FIXME - do not use type, but var!
		
		if (expressionType == EXPR_INT)
		{
			/*
			* example:
			* 	1 : Label_0
			*/
			// Two ostringstream objects are used because clearing/erasing
			// an ostringstream is not trivial
			std::ostringstream convert; 
			std::ostringstream labelConvert; 

			int caseIntegerValue = value.Integer;
			convert << caseIntegerValue;
			mainMethod += "\t\t" + convert.str();

			labelConvert << labelIndex;
			std::string label = "Label_" + labelConvert.str();
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
		GET_EXPR_FROM_PAIR(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod);
	} // end for-loop for pairsAltsLabels vector
	
	// Handle default label
	mainMethod += "DLABEL: \n"; 
	mainMethod += "return\n";
}

void AST::EXPR::generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{
	// TODO
	// Generate appropriate subroutine if not there already
	// then make a call to this subroutine
	// and get the result
	
	// two variables
	mainMethod += "\tinvokestatic " + JVMByteCodeGenerator::ADD_SUBROUTINE + "(II)I\n";
	if (!bytecodeGenerator->isAddSubroutineEnabled())
	{
		bytecodeGenerator->addSubroutine(jasminProgram);
	}
	mainMethod += "\tpop\n"; // FIXME - or "istore_"
}

void AST::EXPR::generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod)
{	
	std::map< std::string, std::pair<int, AST::EXPRESSION_TYPE> > env = bytecodeGenerator->getEnvironment();
	unsigned int environmentSize = env.size();
	std::string* ID = _uValue.exprNewVar.ID;
	boost::shared_ptr<EXPR>* expr = _uValue.exprNewVar.expr;

	if ((*expr)->getExprType() == EXPR_INT)
	{
		int value = (*expr)->getValue().Integer;
		mainMethod += getIntByteCode(bytecodeGenerator, value) + std::string("\t; Var: ") + *ID + std::string("\n");

		std::ostringstream convert; 
		convert << environmentSize;
		std::string storeBytecode = "istore_" + convert.str();
		mainMethod += storeBytecode + "\n";

		updateEnvironment(bytecodeGenerator, ID, environmentSize, EXPR_INT);
	}

}

void AST::EXPR::updateEnvironment(JVMByteCodeGenerator* bytecodeGenerator, std::string* ID, int index, AST::EXPRESSION_TYPE exprType)
{
	std::map< std::string, std::pair<int, AST::EXPRESSION_TYPE> > env = bytecodeGenerator->getEnvironment();
	// Update environment
	std::pair< 
		std::map< std::string, std::pair<int, AST::EXPRESSION_TYPE> >::iterator,
		bool> pair = env.insert(std::make_pair<std::string, std::pair<int, AST::EXPRESSION_TYPE> > 
								(*ID, std::make_pair<int, AST::EXPRESSION_TYPE>(index, exprType)));
		printf("updating last");
	bytecodeGenerator->updateLast(pair.first);
}