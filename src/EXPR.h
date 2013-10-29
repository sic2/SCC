#pragma once

#include "AST.h"

#include "JVMByteCodeGenerator.h"

/*
* The state context is used to pass 
* information through EXPRs that are executed recursevely. 
*/
typedef struct stateContext
{
	std::string typeID;
	std::string ID;
	int stackLocation;
	bool onStack;
} stateContext; 

class AST::EXPR
{
public:

	EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value);
	virtual ~EXPR() {}

	/**
	* @param
	* @param
	* @param
	* @param onStack Set to true the first time
	* @return
	*/
	void generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, 
									std::string& mainMethod, bool onStack, void* context);

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

	/* --------- *
	 * FUNCTIONS *
	 * --------- */
	/*
	* Utility functions
	*/
	std::string integerToString(int value);
	std::string boolToString(bool value);
	std::string getIStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator);
	std::string getIntByteCode(int Integer);
	std::string AST::EXPR::getStrCmpByteCode();

	/*
	* Bytecode generators
	*/
	void generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateBoolByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);
	void generateConstructByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);	
	void generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, void* context);


	void stringPatternMatching(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod,
	 			bool onStack, void* context, std::string condition, std::vector< boost::shared_ptr<ALT> > alternatives);

	AST::EXPRESSION_TYPE evaluateBiOpOperands(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, 
		bool onStack, void* context, boost::shared_ptr<EXPR> operand);

	/*
	* The following functions set fields for a given object
	*/
	int newGenericObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, std::string ID, std::string typeID);
	void updateTags(std::string& mainMethod, int labelIndex, std::string typeID, std::string constructorID);

	int createPrimitiveObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, void* context, std::string label);

	void loadIntToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context);
	void loadBoolToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context);
	void loadStrToObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int arrayIndex, int object, void* context);
	void loadObjectToObject(std::string& mainMethod, int labelIndex, int arrayIndex, int object);
};