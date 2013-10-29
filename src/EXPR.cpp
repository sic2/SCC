// scc header files
#include "OPERATOR.h"
#include "EXPR.h"
#include "ALT.h"
#include "TYPE.h"
#include "CONSTR.h"

#include "JVMByteCodeGenerator.h"

// Std header files
#include <sstream>
#include <map>

/*
* MACROS
*/
#define GET_Condition_FROM_ALT(iterator) ((* iterator)->getCondition())
#define GET_EXPR_FROM_ALT(iterator) ((* iterator)->getEXPR())

AST::EXPR::EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value) 
{
	_typeExpr = typeExpr;
	_uValue = value;
}

void AST::EXPR::generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
						std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{
	switch (_typeExpr)
	{
	case EXPR_INT: generateIntByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		break;
	case EXPR_BOOL: generateBoolByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		break;
	case EXPR_STRING:
		{
			// TODO
		}
		break;
	case EXPR_VAR_CONSTR:
		{
			generateConstructByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
			bytecodeGenerator->addGenericClassForADTs();
		}
		break;
	case EXPR_CASE: generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		break;
	case EXPR_FOR_LOOP:
		{
			// TODO
		}
		break;
	case EXPR_BI_OP: generateBiOPByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		break;
	case EXPR_GROUP:
		{
			std::vector< boost::shared_ptr<EXPR> > expressions = boost::get< Expr_Group >(_uValue).expressions;
			for(std::vector< boost::shared_ptr<EXPR> >::iterator it = expressions.begin(); it != expressions.end(); ++it) 
			{
    			(*it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
			}
		}
		break;
	case EXPR_TYPE_DEF: // No bytecode is generated on typedef, but environment is updated
		{
			Expr_Typedef typeDefinition = boost::get< Expr_Typedef >(_uValue);
			bytecodeGenerator->addTypedef(typeDefinition.ID, typeDefinition);
		}
		break;
	case EXPR_NEW_VAR: // create instance of given type, assuming this was previously defined with typedef
			generateNewVarByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		break;
	default:
		if(DEBUG_MODE >= 1) printf("Error on getting Jasmin Bytecode from an EXPR\n");
	  break;
	} // end switch
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
	std::vector< boost::shared_ptr<ALT> > alternatives = boost::get< Expr_Case >(_uValue).alternatives;

	EXPRESSION_TYPE type = expr->getExprType();

	std::string ID;
	switch (type)
	{
		case EXPR_VAR_CONSTR:
			ID = boost::get< Expr_Var_Constr >(expr->getValue()).ID;
			/*
			Check environment, if nothing than just do a string comparison
			Otherwise, do a constructor pattern matching
			*/
			if (!bytecodeGenerator->typeIsDefined(ID))
			{
				stringPatternMatching(bytecodeGenerator, jasminProgram, mainMethod, onStack, context, ID, alternatives);
			}
			else
			{	
				printf("type is defined\n");
			}
			break;
		break;
		default:	
			printf("Error: case does not support the type %d\n", type);
		break;
	} // end switch
}

/*
* Generate appropriate subroutine if not there already
* then make a call to this subroutine
* and get the result
*/
void AST::EXPR::generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{	
	// Generate bytecode for operands
	EXPRESSION_TYPE op0Type = evaluateBiOpOperands(bytecodeGenerator, jasminProgram, mainMethod, true, context, boost::get< Expr_Bi_Op >(_uValue).expr);
	EXPRESSION_TYPE op1Type = evaluateBiOpOperands(bytecodeGenerator, jasminProgram, mainMethod, true, context, boost::get< Expr_Bi_Op >(_uValue).expr1);

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
		//int loc = ((stateContext*)context)->stackLocation;
		expr->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		bytecodeGenerator->updateEnvironment(&ID, EXPR_NEW_VAR, true);
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
	std::string constructorID = boost::get< Expr_Var_Constr >(getValue()).ID;
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
		std::vector< boost::shared_ptr<EXPR> > exprs = boost::get< Expr_Var_Constr >(getValue()).expressions;
		for (std::vector< boost::shared_ptr<AST::EXPR> >::iterator it = exprs.begin(); it != exprs.end(); ++it)
		{
			AST::EXPRESSION_TYPE exprType = (*it)->getExprType();
			mainMethod += std::string("; loading expr type ") + integerToString(exprType) 
						+ std::string(" to ") + typeID 
						+ std::string(" ") + constructorID + "\n";
			switch (exprType)
			{
				case EXPR_INT:
					loadIntToObject(bytecodeGenerator, mainMethod, (*it), arrayIndex, 
						((stateContext*)context)->stackLocation, context);
					break;
				case EXPR_BOOL:
					loadBoolToObject(bytecodeGenerator, mainMethod, (*it), arrayIndex, 
						((stateContext*)context)->stackLocation, context);
					break;
				case EXPR_STRING:
					loadStrToObject(bytecodeGenerator, mainMethod, (*it), arrayIndex, 
						((stateContext*)context)->stackLocation, context);
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

/*
* TODO
*/
void AST::EXPR::stringPatternMatching(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context,
	std::string condition, std::vector< boost::shared_ptr<ALT> > alternatives)
{
	// Initialise register for cases
	int caseValue = bytecodeGenerator->getLastExpression()->locationInStack;
	mainMethod += "\t " + getIntByteCode(caseValue) + "\n";
	mainMethod += "\t istore " + integerToString(caseValue) + "\n";

	for (std::vector< boost::shared_ptr<AST::ALT> >::iterator it = alternatives.begin(); it != alternatives.end(); ++it)
	{
		if (it != alternatives.begin())
		{
			mainMethod += "Label_" + integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");
		}

		mainMethod += "\t ldc \"" + condition + std::string("\"\n");
		mainMethod += "\t ldc \"" + GET_Condition_FROM_ALT(it)->getID() + std::string("\"\n");
		mainMethod += "\t invokevirtual java/lang/String.equals(Ljava/lang/Object;)Z \n";

		std::string label = "Label_" + integerToString(bytecodeGenerator->nextLabel());
		mainMethod += "\t ifeq " + label + std::string("\n"); 
		GET_EXPR_FROM_ALT(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false, context);
	}
	mainMethod += "Label_" + integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, true); // FIXME - make this work for anything
}

AST::EXPRESSION_TYPE AST::EXPR::evaluateBiOpOperands(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context, boost::shared_ptr<EXPR> operand)
{
	operand->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context); // Push to stack, so that next operand can be pushed too
	EXPRESSION_TYPE opType = bytecodeGenerator->getLastExpression()->type;
	int stackLocation = bytecodeGenerator->getLastExpression()->locationInStack;
	mainMethod += std::string("\tiload ") + integerToString(stackLocation) + std::string("\n"); // FIXME - assume integer
	return opType;
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
	bytecodeGenerator->addNewGenericObject(ID, labelIndex); // Update environment

	// Initialise array of objs
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\tbipush 10 \n"; // FIXME - do not assume size of array to be 10
	mainMethod += "\tanewarray ADTByteCode\n";
	mainMethod += "\tputfield ADTByteCode/objs [LADTByteCode;\n"; 
	return labelIndex;
}

int AST::EXPR::createPrimitiveObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, void* context, std::string label)
{
	int labelIndex = newGenericObject(bytecodeGenerator, mainMethod, label, label);
	((stateContext*)context)->stackLocation = labelIndex;
	// Update the tags fields for this object
	updateTags(mainMethod, labelIndex, label, label); 

	return labelIndex;
}

void AST::EXPR::loadIntToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context)
{
	int labelIndex = createPrimitiveObject(bytecodeGenerator, mainMethod, context, "Int");
	mainMethod += "; Set up value int " + getIntByteCode(boost::get< int >(expr->getValue())) + "\n";
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + getIntByteCode(boost::get< int >(expr->getValue())) + "\n";
	mainMethod += "\tputfield ADTByteCode/intVal I \n";	
}

void AST::EXPR::loadBoolToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context)
{
	int labelIndex = createPrimitiveObject(bytecodeGenerator, mainMethod, context, "Bool");
	mainMethod += "\taload " + integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + boolToString(boost::get< bool >(expr->getValue()));
	mainMethod += "\tputfield ADTByteCode/boolVal Z; \n";
}

void AST::EXPR::loadStrToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context)
{
	int labelIndex = createPrimitiveObject(bytecodeGenerator, mainMethod, context, "String");
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

std::string AST::EXPR::getStrCmpByteCode()
{
	// TODO
	/*
 20:	aload_0
   21:	aload_1
   22:	invokevirtual	#4; //Method java/lang/String.equals:(Ljava/lang/Object;)Z
   25:	ifeq	31
	*/
	return std::string(" ");
}
