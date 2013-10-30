// scc header files
#include "OPERATOR.h"
#include "EXPR.h"
#include "ALT.h"
#include "TYPE.h"
#include "CONSTR.h"

#include "JVMByteCodeGenerator.h"

#include "Helper.h"

// Std header files
#include <sstream>
#include <map>

/*
* MACROS
*/
#define GET_Condition_FROM_ALT(iterator) ((* iterator)->getCondition())
#define GET_EXPR_FROM_ALT(iterator) ((* iterator)->getEXPR())

#define DEFAULT_RETURN_VALUE -1

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
	case EXPR_STRING: generateStringByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		break;
	case EXPR_VAR_CONSTR:
		{
			generateConstructByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
			bytecodeGenerator->addGenericClassForADTs();
		}
		break;
	case EXPR_CASE: generateCaseByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
		break;
	case EXPR_FOR_LOOP: generateForByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
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
	std::string bytecode = getIntByteCode(bytecodeGenerator, mainMethod, boost::get< int >(_uValue));
	bytecodeGenerator->formatJasminInstruction(bytecode);
	mainMethod += bytecode;

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, onStack);
}

void AST::EXPR::generateBoolByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{
	mainMethod += boolToString(bytecodeGenerator, mainMethod, boost::get< bool >(_uValue)) + "\n";
	std::string ID = "";  // ID not relevant
	bytecodeGenerator->updateEnvironment(&ID, EXPR_BOOL, onStack);
}

void AST::EXPR::generateStringByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{
	mainMethod += stringToByteCode(bytecodeGenerator, mainMethod, boost::get< std::string >(_uValue)) + "\n";
	std::string ID = "";  // ID not relevant
	bytecodeGenerator->updateEnvironment(&ID, EXPR_STRING, onStack);
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
		{
			/*
			Check environment, if nothing than just do a string comparison
			Otherwise, do a constructor pattern matching
			*/
			ID = boost::get< Expr_Var_Constr >(expr->getValue()).ID;
			if (!bytecodeGenerator->objIsDefined(ID))
			{
				stringPatternMatching(bytecodeGenerator, jasminProgram, mainMethod, onStack, context, ID, alternatives);
			}
			else
			{	
				objectPatternMatching(bytecodeGenerator, jasminProgram, mainMethod, onStack, context, ID, alternatives);
			}
			break;
		}
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
	((stateContext*)context)->forceLoad = true;

	EXPRESSION_TYPE op0Type = evaluateBiOpOperands(bytecodeGenerator, jasminProgram, mainMethod, true, context, boost::get< Expr_Bi_Op >(_uValue).expr);
	EXPRESSION_TYPE op1Type = evaluateBiOpOperands(bytecodeGenerator, jasminProgram, mainMethod, true, context, boost::get< Expr_Bi_Op >(_uValue).expr1);
	((stateContext*)context)->forceLoad = false;
	(boost::get< Expr_Bi_Op >(_uValue).op)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, op0Type, op1Type);
	EXPRESSION_TYPE exprType = bytecodeGenerator->getLastExpression()->type;

	std::string ID = "SUBROUTINE";
	bytecodeGenerator->updateEnvironment(&ID, exprType, true);
}

void AST::EXPR::generateForByteCode(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context)
{
	// TODO
	
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

	int arrayIndex = 0;
	std::vector< boost::shared_ptr<EXPR> > exprs = boost::get< Expr_Var_Constr >(getValue()).expressions;

	int labelIndex = newGenericObject(bytecodeGenerator, mainMethod, objID, typeID, exprs.size());
	((stateContext*)context)->stackLocation = labelIndex;
	// Update the tags fields for this object
	updateTag(bytecodeGenerator, mainMethod, labelIndex, constructorID);
	
	for (std::vector< boost::shared_ptr<AST::EXPR> >::iterator it = exprs.begin(); it != exprs.end(); ++it)
	{
		AST::EXPRESSION_TYPE exprType = (*it)->getExprType();
		mainMethod += std::string("; loading expr type ") + Helper::instance().integerToString(exprType) 
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
		loadObjectToObject(bytecodeGenerator, mainMethod, labelIndex, arrayIndex, ((stateContext*)context)->stackLocation);
		arrayIndex++;
	}
	
}

void AST::EXPR::stringPatternMatching(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context,
	std::string condition, std::vector< boost::shared_ptr<ALT> > alternatives)
{
	for (std::vector< boost::shared_ptr<AST::ALT> >::iterator it = alternatives.begin(); it != alternatives.end(); ++it)
	{
		if (it != alternatives.begin())
		{
			mainMethod += "Label_" + Helper::instance().integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");
		}

		mainMethod += "\t" + stringToByteCode(bytecodeGenerator, mainMethod, condition) + "\n";
		mainMethod += "\t" + stringToByteCode(bytecodeGenerator, mainMethod, GET_Condition_FROM_ALT(it)->getID()) + "\n";
		mainMethod += "\t invokevirtual java/lang/String.equals(Ljava/lang/Object;)Z \n";

		std::string label = "Label_" + Helper::instance().integerToString(bytecodeGenerator->nextLabel());
		mainMethod += "\t ifeq " + label + std::string("\n"); 
		GET_EXPR_FROM_ALT(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false, context);
		
		mainMethod += "goto def \n";
	}
	mainMethod += "Label_" + Helper::instance().integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");
	mainMethod += getIntByteCode(bytecodeGenerator, mainMethod, DEFAULT_RETURN_VALUE) + "\n";
	mainMethod += "def: \n";

	std::string ID = "";
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, true); // FIXME - make this work for anything
}

void AST::EXPR::objectPatternMatching(JVMByteCodeGenerator* bytecodeGenerator, 
	std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context,
	std::string condition, std::vector< boost::shared_ptr<ALT> > alternatives)
{
	mainMethod += ";CASE STATEMENT START \n";

	int conditionStackLocation = bytecodeGenerator->getObj(condition).second;
	AST::Expr_Typedef conditionTypeDef = bytecodeGenerator->getTypeDef(bytecodeGenerator->getObj(condition).first); 
	for (std::vector< boost::shared_ptr<AST::ALT> >::iterator it = alternatives.begin(); it != alternatives.end(); ++it)
	{
		if (it != alternatives.begin())
		{
			mainMethod += "Label_" + Helper::instance().integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");
		}
		
		// Create object for alternative.
		// This will allow object assignments and comparisons
		int labelIndex = bytecodeGenerator->getEnvironmentSize();
		std::string ID = GET_Condition_FROM_ALT(it)->getID();
		createTmpObject(bytecodeGenerator, mainMethod, ID, labelIndex, conditionTypeDef, GET_Condition_FROM_ALT(it)->getTypes().size());
		mainMethod += "aload " + Helper::instance().integerToString(conditionStackLocation) + "\n";
		mainMethod += "aload " + Helper::instance().integerToString(labelIndex) + "\n";
		mainMethod += "invokevirtual ADTByteCode.COMPARE(LADTByteCode;)Z \n";
		
		std::string label = "Label_" + Helper::instance().integerToString(bytecodeGenerator->nextLabel());
		mainMethod += "\t ifeq " + label + std::string("\n"); 
		
		// XXX - needed? 
		// Map each param for this case to stored value defined by condition
		mapParams(GET_Condition_FROM_ALT(it)->getTypes(), conditionStackLocation, conditionTypeDef, GET_Condition_FROM_ALT(it)->getID());

		GET_EXPR_FROM_ALT(it)->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, false, context);
		
		mainMethod += "goto def \n";
	}
	mainMethod += "Label_" + Helper::instance().integerToString(bytecodeGenerator->currentLabel()) + std::string(":\n");
	mainMethod += getIntByteCode(bytecodeGenerator, mainMethod, DEFAULT_RETURN_VALUE) + "\n";
	mainMethod += "def: \n";
	
	mainMethod += ";CASE STATEMENT END \n";
}

void AST::EXPR::mapParams(std::vector< boost::shared_ptr<AST::TYPE> > params, int conditionStackLocation,
						  AST::Expr_Typedef conditionTypeDef, std::string conditionConstructID)
{

	int noDefinedParams = conditionTypeDef.constructors[0]->getTypes().size();
	if (noDefinedParams == params.size())
	{
		int paramIndex = 0;
		for (std::vector< boost::shared_ptr<AST::TYPE> >::iterator it = params.begin(); it != params.end(); ++it)
		{
			// TODO
			paramIndex++;
		}
	}
	else
	{
		printf("Error: number of params in case and number of params in typedef does not match\n");
	}
}

AST::EXPRESSION_TYPE AST::EXPR::evaluateBiOpOperands(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context, boost::shared_ptr<EXPR> operand)
{
	operand->generateByteCode(bytecodeGenerator, jasminProgram, mainMethod, onStack, context);
	EXPRESSION_TYPE opType = bytecodeGenerator->getLastExpression()->type;
	return opType;
}

/************************
* ADT Utility functions *
*************************/
int AST::EXPR::newGenericObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod,
	std::string ID, std::string typeID, int noObjs)
{
	mainMethod += "\n\tnew ADTByteCode ; create new generic object for type " + typeID + "\n";
	mainMethod += "\tdup \n";
	mainMethod += "\tinvokespecial ADTByteCode.<init>()V \n";
	int labelIndex = bytecodeGenerator->getEnvironmentSize();
	mainMethod += getAStoreByteCode(bytecodeGenerator, mainMethod) + std::string("\n");

	// Update environment
	bytecodeGenerator->addNewGenericObject(ID, labelIndex, typeID); 
	bytecodeGenerator->updateEnvironment(&ID, EXPR_INT, true); // FIXME - make this work for anything

	// Update field about size of objects array
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + getIntByteCode(bytecodeGenerator, mainMethod, noObjs) + "\n";
	mainMethod += "\tputfield ADTByteCode/noObjs I \n";	

	// Initialise array of objs
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\tbipush " + Helper::instance().integerToString(noObjs) + "\n";
	mainMethod += "\tanewarray ADTByteCode\n";
	mainMethod += "\tputfield ADTByteCode/objs [LADTByteCode;\n"; 
	return labelIndex;
}

void AST::EXPR::createTmpObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, 
								std::string& ID, int labelIndex,
								AST::Expr_Typedef conditionTypeDef, int noParams)
{
	mainMethod += ";Start Create ALT obj \n";
	
	mainMethod += "\n\tnew ADTByteCode \n";
	mainMethod += "\tdup \n";
	mainMethod += "\tinvokespecial ADTByteCode.<init>()V \n";
	mainMethod += getAStoreByteCode(bytecodeGenerator, mainMethod) + std::string("\n");
	
	// Update environment
	bytecodeGenerator->addNewTmpObject(ID, labelIndex, conditionTypeDef.ID); 
	bytecodeGenerator->updateEnvironment(&ID, EXPR_NEW_VAR, true);
	
	// Update field about size of objects array
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + getIntByteCode(bytecodeGenerator, mainMethod, noParams) + "\n"; 
	mainMethod += "\tputfield ADTByteCode/noObjs I \n";	
	
	// Initialise array of objs
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\tbipush " + Helper::instance().integerToString(noParams) + "\n";
	mainMethod += "\tanewarray ADTByteCode\n";
	mainMethod += "\tputfield ADTByteCode/objs [LADTByteCode;\n"; 
	
	updateTag(bytecodeGenerator, mainMethod, labelIndex, ID);
	
	mainMethod += ";End Create ALT obj \n";
	
}

int AST::EXPR::createPrimitiveObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, void* context, std::string label)
{
	int labelIndex = newGenericObject(bytecodeGenerator, mainMethod, label, label, 0); // Primitive contains no objects
	((stateContext*)context)->stackLocation = labelIndex;
	updateTag(bytecodeGenerator, mainMethod, labelIndex, label); // Update the tags fields for this object
	return labelIndex;
}

void AST::EXPR::loadIntToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context)
{
	int labelIndex = createPrimitiveObject(bytecodeGenerator, mainMethod, context, "Int");
	mainMethod += "; Set up value int " + getIntByteCode(bytecodeGenerator, mainMethod, boost::get< int >(expr->getValue())) + "\n";
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + getIntByteCode(bytecodeGenerator, mainMethod, boost::get< int >(expr->getValue())) + "\n";
	mainMethod += "\tputfield ADTByteCode/intVal I \n";	
	
	if (((stateContext*)context)->forceLoad)
	{
		mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
		mainMethod += "getfield ADTByteCode/intVal I\n";
	}
}

void AST::EXPR::loadBoolToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context)
{
	int labelIndex = createPrimitiveObject(bytecodeGenerator, mainMethod, context, "Bool");
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + boolToString(bytecodeGenerator, mainMethod, boost::get< bool >(expr->getValue())) + "\n";
	mainMethod += "\tputfield ADTByteCode/boolVal Z; \n";
	
	if (((stateContext*)context)->forceLoad)
	{
		mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
		mainMethod += "getfield ADTByteCode/boolVal Z\n";
	}
}

void AST::EXPR::loadStrToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context)
{
	int labelIndex = createPrimitiveObject(bytecodeGenerator, mainMethod, context, "String");
	mainMethod += "; Set up value string \"" + boost::get< std::string >(expr->getValue()) + "\"\n";
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + stringToByteCode(bytecodeGenerator, mainMethod, boost::get< std::string >(expr->getValue())) + "\n";
	mainMethod += "\tputfield ADTByteCode/strVal Ljava/lang/String; \n";
	
	if (((stateContext*)context)->forceLoad)
	{
		mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
		mainMethod += "getfield ADTByteCode/strVal Ljava/lang/String; \n";
	}
}

void AST::EXPR::loadObjectToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, int labelIndex, int arrayIndex, int object)
{
	mainMethod += "; LOAD OBJ " + Helper::instance().integerToString(object) + std::string(" to array of objs in ") + 
					Helper::instance().integerToString(labelIndex) +  " in position " + Helper::instance().integerToString(arrayIndex) + "\n";			
	mainMethod += "aload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "getfield ADTByteCode/objs [LADTByteCode; \n";
	mainMethod += getIntByteCode(bytecodeGenerator, mainMethod, arrayIndex) + "\n";
	mainMethod += "aload " + Helper::instance().integerToString(object) + std::string("\n");
	mainMethod += "aastore\n";
}

void AST::EXPR::updateTag(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, int labelIndex, std::string constructorID)
{
	mainMethod += "; Set constructorID tag\n";
	mainMethod += "\taload " + Helper::instance().integerToString(labelIndex) + std::string("\n");
	mainMethod += "\t" + stringToByteCode(bytecodeGenerator, mainMethod, constructorID) + "\n";
	mainMethod += "\tputfield ADTByteCode/constrTag Ljava/lang/String; \n";
}

/*
* General Utility functions
*/
std::string AST::EXPR::getAStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod)
{
	bytecodeGenerator->updateProgramEnv(mainMethod, 3);
	return "\tastore " + Helper::instance().integerToString(bytecodeGenerator->getEnvironmentSize());
}

std::string AST::EXPR::boolToString(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, bool value)
{
	bytecodeGenerator->updateProgramEnv(mainMethod, 1); 
	return value ? "\ticonst_1" : "\ticonst_0";
}

std::string AST::EXPR::stringToByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, std::string str)
{
	bytecodeGenerator->updateProgramEnv(mainMethod, 2); 
	return "ldc \"" + str + "\"";
}

std::string AST::EXPR::getIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, int Integer)
{
	bytecodeGenerator->updateProgramEnv(mainMethod, 0); // FIXME - use constant
	std::string retval = "iconst_";
	if (Integer == -1)
	{
		return "iconst_m1";
	}
	else if (Integer < -1 || Integer > 5)
	{
		retval = "bipush ";
	}

	return retval + Helper::instance().integerToString(Integer);
}