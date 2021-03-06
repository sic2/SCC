#pragma once

#include "AST.h"

#include "JVMByteCodeGenerator.h"

/**
* The state context is used to pass 
* information through EXPRs that are executed recursevely. 
*/
typedef struct stateContext
{
	std::string typeID;
	std::string ID;
	int stackLocation;
	bool forceLoad;
} stateContext; 

/**
 * This class represent an EXPR.
 * An EXPR, in the Case language, can assume various forms.
 * A Case program is an EXPR. 
 * Being an EXPR the main entity of a program, all the translation
 * from the AST to the JVM bytecode is done within this class. 
*/
class AST::EXPR
{
public:

	EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value);
	virtual ~EXPR() {}

	/*
	* Fields Getters
	*/
	AST::EXPRESSION_TYPE getExprType() { return this->_typeExpr; }
	AST::uValue getValue() { return this->_uValue; }

private:
	/*
	* EXPR Fields
	*/
	AST::EXPRESSION_TYPE _typeExpr;
	AST::uValue _uValue; 

	
/* --------------------- *
 * Functions to generate *
 * JVM Bytecode			 *
 * --------------------- */
public:
	/**
	 * @param
	 * @param
	 * @param
	 * @param onStack Set to true the first time
	 * @return
	 */
	void generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, 
						  std::string& mainMethod, bool onStack, void* context);
	
private:

	/* --------- *
	 * FUNCTIONS *
	 * --------- */
	/*
	* Utility functions
	*/
	std::string boolToString(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, bool value);
	std::string getIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, int Integer);
	std::string stringToByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, std::string str);
	std::string getAStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod);

	/*
	* Bytecode generators
	*/
	void generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateBoolByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateStringByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateConstructByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);	
	void generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateForByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	
	/*
	* Pattern matching subroutines
	*/
	void stringPatternMatching(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod,
	 			bool onStack, void* context, std::string condition, std::vector< boost::shared_ptr<ALT> > alternatives);
	void objectPatternMatching(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod,
	 			bool onStack, void* context, std::string condition, std::vector< boost::shared_ptr<ALT> > alternatives);
	void createTmpObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, std::string& ID, int labelIndex,
									AST::Expr_Typedef conditionTypeDef, int noParams);

	/*
	* Bi-Operations subroutine
	*/
	AST::EXPRESSION_TYPE evaluateBiOpOperands(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, 
		bool onStack, void* context, boost::shared_ptr<EXPR> operand);

	/*
	* The following functions set fields for a given object
	*/
	int newGenericObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, std::string ID, std::string typeID, int noObjs);
	void updateTag(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, int labelIndex, std::string constructorID);

	int createPrimitiveObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, void* context, std::string label);

	void loadIntToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context);
	void loadBoolToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context);
	void loadStrToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context);
	void loadObjectToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, int labelIndex, int arrayIndex, int object);
};