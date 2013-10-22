#include "AST.h"
#include "JVMByteCodeGenerator.h"

#include <sstream>
#include <map>

#define INTEGER_BASE 10
#define MAX_NUMBER_DIGITS 32

AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value) 
{
	_typeExpr = typeExpr;
	_uValue = value;
}

// EXPR_VAR_CONSTR
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, std::string ID, AST::EXPR* expr)
{
	_typeExpr = typeExpr;
    _id = ID;
    _expr0 = expr;
}

// EXPR_CASE
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::EXPR* expr, std::vector< boost::shared_ptr<ALT> >& alternatives)
{
	_typeExpr = typeExpr;
	_expr0 = expr;
	_alternatives = alternatives;
}

// EXPR_FOR_LOOP
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, std::string ID, AST::EXPR* expr0, AST::EXPR* expr1)
{
	_typeExpr = typeExpr;
	_id = ID;
	_expr0 = expr0;
	_expr1 = expr1;
}

// EXPR_BI_OP
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::EXPR* expr0, std::string op, AST::EXPR* expr1) 
{
	_typeExpr = typeExpr;
	_expr0 = expr0;
	_expr1 = expr1;
	if (op.compare("+") == 0)
	{
		 _operand = OP_ADDITION;
	}
	else if (op.compare("-") == 0)
	{
		_operand = OP_SUBTRACTION;
	}
	else if (op.compare("*") == 0)
	{
		_operand = OP_MULTIPLICATION;
	}
	else if (op.compare("div") == 0)
	{
		_operand = OP_DIVISION;
	}
	else if (op.compare("==") == 0)
	{
		_operand = OP_EQUALITY;
	}
	else if (op.compare("<") == 0)
	{
		_operand = OP_LESS;
	}
	else if (op.compare("or") == 0 || op.compare("||") == 0)
	{
		_operand = OP_OR;
	}
	else if (op.compare("and") == 0 || op.compare("&&") == 0)
	{
		_operand = OP_AND;
	}
	else if (op.compare("..") == 0)
	{
		_operand = OP_RANGE;
	}
	else
	{
		if(DEBUG_MODE >= 1)
		{
			printf("Operand %d Unknown\n", _operand);
		}
	}

}

// EXPR_GROUP 
EXPR(EXPRESSION_TYPE typeExpr, std::vector< boost::shared_ptr<EXPR> >& expressions)
{
	_typeExpr = typeExpr;
	_expressions = expressions;
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
			delete _expr0;
		}
		break;
	case EXPR_CASE:
		{
			delete _expr0;
		}
		break;
	case EXPR_FOR_LOOP:
	case EXPR_BI_OP:
		{
			delete _expr0;
			delete _expr1;
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

void AST::EXPR::generateByteCode(std::string& output)
{
	switch(_typeExpr)
	{
	case EXPR_INT:
		{
			std::string bytecode = getIntByteCode(_uValue.Integer);
			JVMByteCodeGenerator::formatJasminInstruction(bytecode);
			output += bytecode;
			std::string store = "istore_0"; // FIXME - hardcoded bytecode
			JVMByteCodeGenerator::formatJasminInstruction(store);
			output += store;
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
			// TODO
		}
		break;
	case EXPR_CASE:
		{
			generateCaseByteCode(output);
		}
		break;
	case EXPR_FOR_LOOP:
	case EXPR_BI_OP:
		{
			generateBiOPByteCode(output);
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
void AST::EXPR::generateCaseByteCode(std::string& output) 
{
	_expr0->generateByteCode(output);
	output += "\tiload_0 \n"; // FIXME - hardcoded bytecode
	output += "\tlookupswitch \n";

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
	for(std::vector< boost::shared_ptr<AST::ALT> >::iterator it = _alternatives.begin(); it != _alternatives.end(); ++it) 
	{
		PRIMITIVE_TYPE primitiveType;
		uValue value;
	
		bool valid = (*it)->getTYPE()->getValue(&primitiveType, &value);
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
				output += "\t\t" + convert.str();

				labelConvert << labelIndex;
				std::string label = "Label_" + labelConvert.str();
				output += " : " + label + "\n";
				labelIndex++;

				// Adding the integer and the label to pairsAltsLabels
				pairsAltsLabels.push_back(std::make_pair< boost::shared_ptr<AST::ALT>, std::string>((*it), label));
			}
		}
	} // end for-loop for _alternatives vector
	// Default label must always be specified - but it won't have
	// any functionality in the Case language
	output += "\t\tdefault : DLABEL\n"; 

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
		output += (*it).second + ": \n";
		(*it).first->getEXPR()->generateByteCode(output);
	} // end for-loop for pairsAltsLabels vector
	// Handle default label
	output += "DLABEL: \n"; 
	output += "return\n";
}

void AST::EXPR::generateBiOPByteCode(std::string& output)
{
	// TODO
}