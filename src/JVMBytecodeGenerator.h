#pragma once

#include "AST.h"

class JVMByteCodeGenerator
{
public:
	/******************
	* Static variables
	*******************/
	// [ TODO ] - static variables for other subroutines

	/**
	* Define the name of the add subroutine to use over all the rest of the program
	*/
	const static std::string ADD_SUBROUTINE;

public:
	JVMByteCodeGenerator(boost::shared_ptr<AST::PROGRAM> program);
	virtual ~JVMByteCodeGenerator() {}

	/**
	* Generates JVM bytecode and output to file
	* @param outFileName 
	* @return 	True if bytecode was generate successfully.
	*			False otherwise.
	*/
	bool generateByteCode(std::string outFileName);

	/**
	* @return true if a subroutine for addition has already been added
	*/
	bool isAddSubroutineEnabled() { return this->_addSubroutineEnabled; }

	/**
	* Append an add subroutine at the end of the given bytecode jasmin program
	* @param bytecodeProgram
	*/
	void addSubroutine(std::string& bytecodeProgram);

	/**
	* Dynamically format an instruction into a jasmin instruction.
	* MACROS cannot be used on dynamic content
	*/
	static void formatJasminInstruction(std::string& instruction);

	/**
	* TODO
	*/
	static void printInt(std::string& output, int var);

private:
	boost::shared_ptr<AST::PROGRAM> _program;

	void addInitialJasminCode(std::string& output);
	void addFinalJasminCode(std::string& output);

	/**
	* True if the add subroutine has been already added to 
	* the Jasmin bytecode
	*/
	bool _addSubroutineEnabled;
};