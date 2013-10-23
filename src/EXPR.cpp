#include "AST.h"
#include "JVMByteCodeGenerator.h"

#include <sstream>
#include <map>

#define INTEGER_BASE 10
#define MAX_NUMBER_DIGITS 32

#define GET_TYPE_FROM_ALT(iterator) (*((* iterator)->getTYPE()))
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

	switch(_typeExpr)
	{
	case EXPR_INT:
	case EXPR_BOOL:
	case EXPR_STRING:
		break;
	case EXPR_VAR_CONSTR:
		{
			// printf("this statement should never be reached\n");
		}
		break;
	case EXPR_CASE:
		{
			// delete _expr0;
		}
		break;
	case EXPR_FOR_LOOP:
		break;
	case EXPR_BI_OP:
		{
			// delete _expr0;
			// delete _expr1;
		}
		break;
	// TODO	
	case EXPR_GROUP:
	case EXPR_TYPE_DEF:
	case EXPR_NEW_VAR:
	default:
		if(DEBUG_MODE >= 1)
		{
	  		printf("Error EXPR Destructor\n");
	  	}
	  break;
	}  // end switch
}

void AST::EXPR::generateByteCode(std::string& jasminProgram, std::string& mainMethod)
{
	printf("\t\t\tgeneraing bytecode %d\n", _typeExpr);
	switch(_typeExpr)
	{
	case EXPR_INT:
		{
			std::string bytecode = getIntByteCode(_uValue.Integer);
			JVMByteCodeGenerator::formatJasminInstruction(bytecode);
			mainMethod += bytecode;
			std::string store = "istore_0"; // FIXME - hardcoded bytecode
			JVMByteCodeGenerator::formatJasminInstruction(store);
			mainMethod += store;
			//JVMByteCodeGenerator::printInt(output, 0);
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
			generateCaseByteCode(jasminProgram, mainMethod);
		}
		break;
	case EXPR_FOR_LOOP:
		break;
	case EXPR_BI_OP:
		{
			generateBiOPByteCode(jasminProgram, mainMethod);
		}
		break;
	// TODO	
	case EXPR_GROUP:
		// TODO
	case EXPR_TYPE_DEF:
		// TODO
	case EXPR_NEW_VAR:
		// TODO
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
std::string AST::EXPR::getIntByteCode(int Integer)
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
void AST::EXPR::generateCaseByteCode(std::string& jasminProgram, std::string& mainMethod) 
{
	boost::shared_ptr<EXPR>* expr = _uValue.exprCase.expr;
	(*expr)->generateByteCode(jasminProgram, mainMethod);

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
		PRIMITIVE_TYPE primitiveType;
		uValue value;
		
		bool valid = GET_TYPE_FROM_ALT(it)->getValue(&primitiveType, &value);
		if (valid)
		{
			if (primitiveType == TYPE_INT)
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
		GET_EXPR_FROM_PAIR(it)->generateByteCode(jasminProgram, mainMethod);
	} // end for-loop for pairsAltsLabels vector
	
	// Handle default label
	mainMethod += "DLABEL: \n"; 
	mainMethod += "return\n";
}

void AST::EXPR::generateBiOPByteCode(std::string& jasminProgram, std::string& mainMethod)
{
	// TODO
	// Generate appropriate subroutine if not there already
	// then make a call to this subroutine
	// and get the result
	//if (!JVMByteCodeGenerator::addSubroutineEnabled)
	//{

	//}
}