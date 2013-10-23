#pragma once

#include "AST.h"
#include <map>

/**********
* MACROS
* Publicly accessible since they are
* useful to other parts of the compiler to 
* create various parts of the jasmin bytecode.
* However, MACROS accepting params cannot be used with 
* runtime generated content. 
**********/

/*
* General spacing and formatting
*/
#define NEW_LINE "\n"
#define TAB "\t"
#define SPACE " "

/*
* JASMIN output formatting 
*/
#define JASMIN_WITHOUT_NEWLINE(stream, x) stream += TAB x
#define JASMIN_WITH_NEWLINE(stream, x) stream += x NEW_LINE
#define JASMIN_DIRECTIVE(stream, x) JASMIN_WITH_NEWLINE(stream, x)
#define JASMIN_LABEL(stream, x) JASMIN_WITH_NEWLINE(stream, SPACE x)
#define JASMIN_INSTR(stream, x) JASMIN_WITH_NEWLINE(stream, TAB x)

/*
* Stack and locals
*/
#define JASMIN_STACK(stream, size) JASMIN_WITH_NEWLINE(stream, TAB ".limit stack " #size)
#define JASMIN_LOCALS(stream, size) JASMIN_WITH_NEWLINE(stream, TAB ".limit locals " #size)

/*
* Invokations
*/
#define INVOKE_SPECIAL_INT "invokespecial java/lang/Object/<init>()V"
#define PRINT_STREAM "getstatic java/lang/System/out Ljava/io/PrintStream;"
#define INVOKE_PRINTLN "invokevirtual java/io/PrintStream/println(I)V"
#define SUBROUTINE(nameSubroutine) ".method public static " #nameSubroutine "()V"

/*
* Subroutines names
*/

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
	* @return environment
	*/
	std::map< std::string, std::pair<int, AST::EXPRESSION_TYPE> > getEnvironment() { return this->_environment; }

	/**
	* Dynamically format an instruction into a jasmin instruction.
	* MACROS cannot be used on dynamic content
	*/
	void formatJasminInstruction(std::string& instruction);

	/**
	* TODO
	*/
	void printInt(std::string& output, int var);

private:
	boost::shared_ptr<AST::PROGRAM> _program;

	void addInitialJasminCode(std::string& output);
	void addInitialMainJasminCode(std::string& output);
	void addFinalMainJasminCode(std::string& output);

	/**
	* True if the add subroutine has been already added to 
	* the Jasmin bytecode
	*/
	bool _addSubroutineEnabled;

	/**
	* The environment or symbol tables
	* is used to map "identifiers to their types and locations"
	* Appel A., Modern Compiler Implementation in C
	*/
	std::map< std::string, std::pair<int, AST::EXPRESSION_TYPE> > _environment;
};