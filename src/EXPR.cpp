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

void AST::EXPR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
						std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{
	//boost::shared_ptr< Expression_Info > retval;
	switch(_typeExpr)
	{
	case EXPR_INT:
		{
			generateIntByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		}
		break;
	case EXPR_BOOL:
		{
			generateBoolByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		}
		break;
	case EXPR_STRING:
		{
			// TODO
		}
		break;
	case EXPR_VAR_CONSTR:
		{
			// create object knowing constructor and etc
			generateConstructByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
			bytecodeGenerator->addGenericClassForADTs();
		}
		break;
	case EXPR_CASE:
		{
			generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		}
		break;
	case EXPR_FOR_LOOP:
		{
			// TODO
		}
		break;
	case EXPR_BI_OP:
		{
			generateBiOPByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		}
		break;
	case EXPR_GROUP:
		{
			// Iterate over all expressions
			std::vector< boost::shared_ptr<EXPR> > expressions = boost::get< Expr_Group >(_uValue).expressions;
			for(std::vector< boost::shared_ptr<EXPR> >::iterator it = expressions.begin(); it != expressions.end(); ++it) 
			{
    			(*it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
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
			generateNewVarByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		}
		break;
	default:
		if(DEBUG_MODE >= 1)
		{
	  		printf("Error on getting Jasmin Bytecode from an EXPR\n");
	  	}
	  break;
	} // end switch

	//return retval;
}

/******************
* Private methods
*******************/
void AST::EXPR::generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{	
	std::string bytecode = getIntByteCode(boost::get< int >(_uValue));
	bytecodeGenerator->formatJasminInstruction(bytecode);
	mainMethod += bytecode;

	std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
	mainMethod += storeBytecode + "\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, onStack);
}

void AST::EXPR::generateBoolByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{
	mainMethod += boolToString(boost::get< bool >(_uValue));
	std::string storeBytecode = getIStoreByteCode(bytecodeGenerator);
	mainMethod += storeBytecode + "\n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_BOOL, onStack);
}

void AST::EXPR::generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context) 
{
	boost::shared_ptr<EXPR> expr = boost::get< Expr_Case >(_uValue).expr;
	expr->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true, context); 	
 	// int caseValue = bytecodeGenerator->getEnvironmentSize(); 

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
			mainMethod += std::string("\tiload ") + integerToString(caseValue - 1) + std::string("\n"); // FIXME - assume integer
			GET_Condition_FROM_ALT(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);
			mainMethod += std::string("\tiload ") + integerToString(caseValue) + std::string("\n"); // FIXME - assume integer

			std::string label = "Label_" + integerToString(bytecodeGenerator->nextLabel());
			mainMethod += std::string("\t if_icmpne ") + label + std::string("\n");
			caseExpr = GET_EXPR_FROM_ALT(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false);
		}
		*/
	} // end for-loop for _alternatives vector
	mainMethod += "Label_" + integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, caseExpr, true);
}

// Generate appropriate subroutine if not there already
// then make a call to this subroutine
// and get the result
// two variables
void AST::EXPR::generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{	
	// Generate bytecode for operands
	boost::shared_ptr<EXPR> operand_0 = boost::get< Expr_Bi_Op >(_uValue).expr;
	operand_0->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true, context); // Push to stack, so that next operand can be pushed too
	EXPRESSION_TYPE op0Type = bytecodeGenerator->getLastExpression()->type;
	int caseValue = bytecodeGenerator->getLastExpression()->locationInStack;
	mainMethod += std::string("\tiload ") + integerToString(caseValue) + std::string("\n"); // FIXME - assume integer
	
	boost::shared_ptr<EXPR> operand_1 = boost::get< Expr_Bi_Op >(_uValue).expr1;
	operand_1->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, true, context); // can be overwritten, so do not update environment stack
	EXPRESSION_TYPE op1Type = bytecodeGenerator->getLastExpression()->type;
	caseValue = bytecodeGenerator->getLastExpression()->locationInStack;
	mainMethod += std::string("\tiload ") + integerToString(caseValue) + std::string("\n"); // FIXME - assume integer

	(boost::get< Expr_Bi_Op >(_uValue).op)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, op0Type, op1Type);
	EXPRESSION_TYPE exprType = bytecodeGenerator->getLastExpression()->type;
	mainMethod += getIStoreByteCode(bytecodeGenerator) + "\n";

	std::string ID = "SUBROUTINE";
	bytecodeGenerator->updateEnvironment(&ID, exprType, true);
}

// new var creates a new variables which is assigned anything that is on the RHS
// for instance kevin :: Person = Person Age 21 Address 0 "String"
// creates a new instance kevin of type person, but
// the person itself is created by EXPR_VAR_CONSTR
// this way it is possible to have expressions like:
// 		ages :: int = kevin.age * 2
void AST::EXPR::generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{	
	std::string ID = boost::get< Expr_New_Var >(_uValue).ID;
	std::string typeID = boost::get< Expr_New_Var >(_uValue).typeID;
	boost::shared_ptr<EXPR> expr = boost::get< Expr_New_Var >(_uValue).expr;

	if (bytecodeGenerator->typeIsDefined(typeID))
	{
		((stateContext*)context)->typeID = typeID;
		((stateContext*)context)->ID = ID;
		
		int loc = ((stateContext*)context)->stackLocation;

		expr->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);

		/*
		* TODO
		* After evaluating the expression map
		* the result of it to a register and update the environment
		*/	
		printf("stack loc %d\n", loc);
	}
	else if (DEBUG_MODE >= 1)
	{
		printf("Error: Type %s not defined\n", typeID.c_str());
	}

}

/*
* Traverse tree defined by constructor from leaves to root
* for each node instantiate a new generic object
*	
* Start by creating a new object for this type_id and constructor_id
* store object at register X
* if type is not primitive, 
* 		then create new generic obj, to be stored at register X + 1
* 		and add to array field in object X
* otherwise 
* 		set field in current object X
*/
void AST::EXPR::generateConstructByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{
	std::string constructorID = boost::get< Expr_Var_Constr >(this->getValue()).ID;
	std::string typeID = ((stateContext*)context)->typeID;
	std::string objID = ((stateContext*)context)->ID;

	bool constructorDefined = false;
	std::vector< boost::shared_ptr<CONSTR> > constructors = bytecodeGenerator->getTypeDef(typeID).constructors;
	for(std::vector< boost::shared_ptr<CONSTR> >::iterator it = constructors.begin(); it != constructors.end(); ++it) 
	{
		if ((*it)->getID().compare(constructorID) == 0)
		{
			constructorDefined = true;
			break;
		}
	}

	if (constructorDefined)
	{
		int labelIndex = newGenericObject(bytecodeGenerator, mainMethod, objID, typeID);
		((stateContext*)context)->stackLocation = labelIndex;

		// Update the tags fields for this object
		updateTags(mainMethod, labelIndex, typeID, constructorID);

		int arrayIndex = 0;
		std::vector< boost::shared_ptr<EXPR> > exprs = boost::get< Expr_Var_Constr >(this->getValue()).expressions;
		for(std::vector< boost::shared_ptr<AST::EXPR> >::iterator it = exprs.begin(); it != exprs.end(); ++it)
		{
			AST::EXPRESSION_TYPE exprType = (*it)->getExprType();
			mainMethod += std::string("; loading expr type ") + integerToString(exprType) 
						+ std::string(" to ") + typeID 
						+ std::string(" ") + constructorID + "\n";
			switch(exprType)
			{
				case EXPR_INT:
					loadIntToObject(bytecodeGenerator, mainMethod, (*it), labelIndex, arrayIndex, ((stateContext*)context)->stackLocation, context);
					break;
				case EXPR_BOOL:
					loadBoolToObject(bytecodeGenerator, mainMethod, (*it), labelIndex, arrayIndex, ((stateContext*)context)->stackLocation, context);
					break;
				case EXPR_STRING:
					loadStrToObject(bytecodeGenerator, mainMethod, (*it), labelIndex, arrayIndex, ((stateContext*)context)->stackLocation, context);
					break;
				case EXPR_NEW_VAR:
					(*it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false, context);
					break;
				default:
					printf("Error: expression type %d not expected\n", exprType);
					break;
			} // End switch
			loadObjectToObject(mainMethod, labelIndex, arrayIndex, ((stateContext*)context)->stackLocation);
			arrayIndex++;
		}
	}
	else if (DEBUG_MODE >= 1)
	{
		printf("Error: Constructor type %s not defined\n", constructorID.c_str());
	}

}


/************************
* ADT Utility functions *
*************************/
int AST::EXPR::newGenericObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod,
	std::string ID, std::string typeID)
{
	mainMethod += "\n\tnew ADTByteCode ; create new generic object for type " + typeID + "\n";
	mainMethod += "\tdup \n";
	mainMethod += "\tinvokespecial ADTByteCode.<init>()V \n";
	int labelIndex = bytecodeGenerator->nextLabel();
	mainMethod += "\tastore_" + integerToString(labelIndex) + std::string("\n");
	// Update environment
	bytecodeGenerator->addNewGenericObject(ID, labelIndex);

	// Initialise array of objs
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\tbipush 10 \n"; // FIXME - do not assume size of array to be 10
	mainMethod += "\tanewarray ADTByteCode\n";
	mainMethod += "\tputfield ADTByteCode/objs [LADTByteCode;\n"; 
	return labelIndex;
}

void AST::EXPR::loadIntToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int labelIndex, int arrayIndex, int object, void* context)
{
	labelIndex = newGenericObject(bytecodeGenerator, mainMethod, "Int", "Int");
	((stateContext*)context)->stackLocation = labelIndex;
	// Update the tags fields for this object
	updateTags(mainMethod, labelIndex, "Int", "Int"); 

	mainMethod += "; Set up value int " + getIntByteCode(boost::get< int >(expr->getValue())) + "\n";
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + getIntByteCode(boost::get< int >(expr->getValue())) + "\n";
	mainMethod += "\tputfield ADTByteCode/intVal I \n";	
}

void AST::EXPR::loadBoolToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int labelIndex, int arrayIndex, int object, void* context)
{
	// TODO


	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + boolToString(boost::get< bool >(expr->getValue()));
	mainMethod += "\tputfield ADTByteCode/boolVal Z; \n";
}

void AST::EXPR::loadStrToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int labelIndex, int arrayIndex, int object, void* context)
{
	labelIndex = newGenericObject(bytecodeGenerator, mainMethod, "String", "String");
	((stateContext*)context)->stackLocation = labelIndex;
	// Update the tags fields for this object
	updateTags(mainMethod, labelIndex, "String", "String");

	mainMethod += "; Set up value string \"" + boost::get< std::string >(expr->getValue()) + "\"\n";
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\tldc \"" + boost::get< std::string >(expr->getValue()) + "\"\n";
	mainMethod += "\tputfield ADTByteCode/strVal Ljava/lang/String; \n";
}

void AST::EXPR::loadObjectToObject(std::string& mainMethod, int labelIndex, int arrayIndex, int object)
{
	mainMethod += "; LOAD OBJ " + integerToString(object) + std::string(" to array of objs in ") + 
					integerToString(labelIndex) +  " in position " + integerToString(arrayIndex) + "\n";			
	mainMethod += "aload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "getfield ADTByteCode/objs [LADTByteCode; \n";
	mainMethod += getIntByteCode(arrayIndex) + "\n";
	mainMethod += "aload " + integerToString(object) + std::string("\n");
	mainMethod += "aastore\n";
}

void AST::EXPR::updateTags(std::string& mainMethod, int labelIndex, std::string typeID, std::string constructorID)
{
	mainMethod += "; Set typeID tag\n";
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\tldc \"" + typeID + "\"\n";
	mainMethod += "\tputfield ADTByteCode/typeTag Ljava/lang/String; \n";

	mainMethod += "; Set constructorID tag\n";
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\tldc \"" + constructorID + "\"\n";
	mainMethod += "\tputfield ADTByteCode/constrTag Ljava/lang/String; \n";
}

/*
* General Utility functions
*/
std::string AST::EXPR::getIStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator)
{
	return "\tistore " + integerToString(bytecodeGenerator->getEnvironmentSize());
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
