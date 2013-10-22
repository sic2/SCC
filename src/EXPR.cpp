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
AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::EXPR* expr, std::vector<ALT> alternatives)
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
			std::string bytecode = getIntByteCode();
			JVMByteCodeGenerator::formatJasminInstruction(bytecode);
			output += bytecode;
			std::string store = "istore_0"; // FIXME - hardcoded bytecode
			JVMByteCodeGenerator::formatJasminInstruction(store);
			output += store;
			JVMByteCodeGenerator::printInt(output, 0);

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
			// TODO
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
std::string AST::EXPR::getIntByteCode()
{
	std::ostringstream convert; 
	convert << _uValue.Integer;

	std::string retval = "iconst_";
	if (_uValue.Integer == -1)
	{
		return "iconst_m1";
	}
	else if (_uValue.Integer < -1 || _uValue.Integer > 5)
	{
		retval = "bipush ";
	}

	return retval + convert.str();
}

// [ ASSUMPTION ] - case of integers
void AST::EXPR::generateCaseByteCode(std::string& output) 
{
	output += "\t istore_0 \n"; // FIXME - hardcoded bytecode
	output += "\t lookupswitch \n";

	// A vector is created to store each ALTernative, its own label as well
	std::vector< std::pair<AST::ALT, std::string> > pairsAltsLabels;  // FIXME - do not limit this to integers
	int labelIndex = 0;
	std::ostringstream convert; 

	/*
	* Loop through all alternatives and print the first part of the lookupswitch
	* example:
	* 1 : Label_0
	* 2 : Label_1
	* 324 : Label_2
	* etc..
	*/
	for(std::vector<AST::ALT>::iterator it = _alternatives.begin(); it != _alternatives.end(); ++it) 
	{
		PRIMITIVE_TYPE primitiveType;
		uValue value;
	
		bool valid = (*it).getTYPE()->getValue(&primitiveType, &value);
		if (valid)
		{
			if (primitiveType == TYPE_INT)
			{
				/*
				* example:
				* 	1 : Label_0
				*/
				int caseIntegerValue = value.Integer;
				convert << caseIntegerValue;
				output += "\t\t " + convert.str();
				convert << labelIndex;
				std::string label = "Label_" + convert.str();
				output += " : " + label + "\n";

				// Adding the integer and the label to pairsAltsLabels
				pairsAltsLabels.push_back(std::make_pair<AST::ALT, std::string>((*it), label));
			}
		}
	} // end for-loop for _alternatives vector

	/* Loop pairsAltsLabels to expand each case 
	* example:
	* Label_0:
		EXPR
	* Label_1:
		EXPR
	* etc...
	*/
	for(std::vector< std::pair<AST::ALT, std::string> >::iterator it = pairsAltsLabels.begin(); it != pairsAltsLabels.end(); ++it) 
	{
		AST::ALT alternative = (*it).first;
		std::string label = (*it).second;
		output += label + ": \n";
		
	} // end for-loop for pairsAltsLabels vector
}
