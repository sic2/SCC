#include "OPERATOR.h"
#include "EXPR.h"
#include "ALT.h"
#include "TYPE.h"
#include "CONSTR.h"

#include "JVMByteCodeGenerator.h"

#include <sstream>
#include <map>

#define GET_Condition_FROM_ALT(iterator) ((* iterator)->getCondition())
#define GET_EXPR_FROM_ALT(iterator) ((* iterator)->getEXPR())

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

AST::EXPRESSION_TYPE AST::EXPR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
						std::string& jasminProgram, std::string& mainMethod, bool onStack)
{
	EXPRESSION_TYPE retval = _typeExpr;
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
			// XXX - maybe this is never used
		}
		break;
	case EXPR_CASE:
		{
			retval = generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
		}
		break;
	case EXPR_FOR_LOOP:
		{
			// TODO
		}
		break;
	case EXPR_BI_OP:
		{
			retval = generateBiOPByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
		}
		break;
	case EXPR_GROUP:
		{
			// Iterate over all expressions
			std::vector< boost::shared_ptr<EXPR> > expressions = boost::get< Expr_Group >(_uValue).expressions;
			for(std::vector< boost::shared_ptr<EXPR> >::iterator it = expressions.begin(); it != expressions.end(); ++it) 
			{
    			(*it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
			}
		}
		break;
	case EXPR_TYPE_DEF:
		{
			// No bytecode is generated on typedef, but environment is updated
			Expr_Typedef typeDefinition = boost::get< Expr_Typedef >(_uValue);
			bytecodeGenerator->addTypedef(typeDefinition.ID, typeDefinition);
		}
		break;
	case EXPR_NEW_VAR:
		{
			// create instance of given type, assuming this was previously defined
			// with typedef
			retval = generateNewVarByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack);
			bytecodeGenerator->addGenericClassForADTs();
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

std::string AST::EXPR::getIntByteCode(int Integer)
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

AST::EXPRESSION_TYPE AST::EXPR::generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack)
{	
	std::string bytecode = getIntByteCode(boost::get< int >(_uValue));
	bytecodeGenerator->formatJasminInstruction(bytecode);
	mainMethod += bytecode;

	std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
	mainMethod += storeBytecode + "\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, onStack);
	return EXPR_INT;
}

AST::EXPRESSION_TYPE AST::EXPR::generateBoolByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack)
{
	mainMethod += boolToString(boost::get< bool >(_uValue));
	std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
	mainMethod += storeBytecode + "\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_BOOL, onStack);

	return EXPR_BOOL;
}

AST::EXPRESSION_TYPE AST::EXPR::generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack) 
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

		/*
		* FIXME
		* be able to compare first expression with types
		* 
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
		*/
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
AST::EXPRESSION_TYPE AST::EXPR::generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack)
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

// TODO - refactor
AST::EXPRESSION_TYPE AST::EXPR::generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack)
{	
	std::string ID = boost::get< Expr_New_Var >(_uValue).ID;
	std::string typeID = boost::get< Expr_New_Var >(_uValue).typeID;
	std::string constructorID = boost::get< Expr_New_Var >(_uValue).constructorID;
	boost::shared_ptr<EXPR> expr = boost::get< Expr_New_Var >(_uValue).expr;

	if (bytecodeGenerator->typeIsDefined(typeID))
	{
		AST::Expr_Typedef typeDefinition = bytecodeGenerator->getTypeDef(typeID);

		// Check if a constructor matching constructorID exist
		bool constructorDefined = false;
		std::vector< boost::shared_ptr<CONSTR> > constructors = typeDefinition.constructors;
		for(std::vector< boost::shared_ptr<CONSTR> >::iterator it = constructors.begin(); it != constructors.end(); ++it) 
		{	
			if ((*it)->getID().compare(constructorID) == 0)
			{
				// TODO 
				/*
				* Traverse tree defined by expr and constructor from leaves to root
				* for each node instantiate a new generic object
				* the object contains:
				* 	- typeTag = typeID
				*	- constrTag = constructorID
				* 	- genericObjects[] = sub types
				*	- values (if any)
				*/

				constructorDefined = true;
				std::vector< boost::shared_ptr<AST::TYPE> > types = (*it)->getTypes();

				// Start by creating a new object for this type_id and constructor_id
				// store object at register X
				// if type is not primitive, 
				// 		then create new generic obj, to be stored at register X + 1
				// 		and add to array field in object X
				// otherwise 
				// 		set field in current object X
				mainMethod += "\n\tnew ADTByteCode ; create new generic object for type " + typeID + "\n";
				mainMethod += "\tdup \n";
				mainMethod += "\tinvokespecial ADTByteCode.<init>()V \n";
				int labelIndex = bytecodeGenerator->nextLabel();
				mainMethod += "\tastore_" + integerToString(labelIndex) + std::string("\n");
				// Update environment
				bytecodeGenerator->addNewGenericObject(ID, labelIndex);

				
				std::vector< boost::shared_ptr<EXPR> > exprs = boost::get< Expr_Group >(expr->getValue()).expressions;
				for(std::vector< boost::shared_ptr<AST::EXPR> >::iterator itt = exprs.begin(); itt != exprs.end(); ++itt)
				{
					// TODO 
					// update tag
					if ((*itt)->getExprType() == EXPR_INT)
					{
						mainMethod += "\taload_" + integerToString(labelIndex) + std::string("\n");
						mainMethod += "\t" + getIntByteCode(boost::get< int >((*itt)->getValue())) + "\n";
						mainMethod += "putfield ADTByteCode/intVal I \n";
					}
					else if ((*itt)->getExprType() == EXPR_STRING)
					{
						mainMethod += "\taload_" + integerToString(labelIndex) + std::string("\n");
						mainMethod += "\tldc " + boost::get< std::string >((*itt)->getValue()) + "\n";
						mainMethod += "\tputfield ADTByteCode/strVal Ljava/lang/String; \n";
					}
					else if ((*itt)->getExprType() == EXPR_BOOL)
					{
						mainMethod += "\taload_" + integerToString(labelIndex) + std::string("\n");
						mainMethod += "\t" + boolToString(boost::get< bool >((*itt)->getValue())); // FIXME 
						mainMethod += "\tputfield ADTByteCode/boolVal Z; \n";
					}
					else
					{
						// TODO
						// initialise array of objs
						(*itt)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);
					}

				}
				break;
			}	
		}

		if (!constructorDefined)
		{
			if(DEBUG_MODE >= 1)
			{
				printf("Error: Constructor %s not defined\n", constructorID.c_str());
			}
		}
	}
	else
	{
		if(DEBUG_MODE >= 1)
		{
			printf("Error: Type %s not defined\n", typeID.c_str());
		}
	}

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

std::string AST::EXPR::boolToString(bool value)
{
	return value ? "\ticonst_1\n" : "\ticonst_0\n";
}