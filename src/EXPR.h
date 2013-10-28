#pragma once

#include "AST.h"
#include "JVMByteCodeGenerator.h"

class AST::EXPR
{
public:

	EXPR(AST::EXPRESSION_TYPE typeExpr, AST::uValue value);
	virtual ~EXPR();

	/**
	* @param
	* @param
	* @param
	* @param onStack Set to true the first time
	* @return
	*/
	EXPRESSION_TYPE generateByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, 
									std::string& mainMethod, bool onStack, int* stackPos);

	/*
	* Getters
	*/
	AST::EXPRESSION_TYPE getExprType() { return this->_typeExpr; }
	AST::uValue getValue() { return this->_uValue; }

private:
	/*
	* Utility functions
	*/
	std::string integerToString(int value);
	std::string boolToString(bool value);
	std::string getIStoreByteCode(JVMByteCodeGenerator* bytecodeGenerator);
	std::string getIntByteCode(int Integer);

	/*
	* Fields
	*/
	AST::EXPRESSION_TYPE _typeExpr;
	AST::uValue _uValue; 

	/*
	* Bytecode generators
	*/
	EXPRESSION_TYPE generateIntByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack);
	EXPRESSION_TYPE generateBoolByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack);
	EXPRESSION_TYPE generateCaseByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack);
	EXPRESSION_TYPE generateBiOPByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack);
	EXPRESSION_TYPE generateNewVarByteCode(JVMByteCodeGenerator* bytecodeGenerator, std::string& jasminProgram, std::string& mainMethod, bool onStack, int* stackPos);

	int newGenericObject(JVMByteCodeGenerator* bytecodeGenerator, std::string& mainMethod,
	std::string ID, std::string typeID);
	void loadIntToObject(std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int labelIndex);
	void loadBoolToObject(std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int labelIndex);
	void loadStrToObject(std::string& mainMethod, boost::shared_ptr<AST::EXPR> expr, int labelIndex);
	void loadObjectToObject(std::string& mainMethod, int labelIndex, int arrayIndex, int object);
	void updateTags(std::string& mainMethod, int labelIndex, std::string typeID, std::string constructorID);
};