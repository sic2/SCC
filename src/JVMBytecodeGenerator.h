#pragma once

#include "AST.h"
#include <map>

/**********************************************************
* MACROS
* Publicly accessible since they are
* useful to other parts of the compiler to 
* create various parts of the jasmin bytecode.
* However, MACROS accepting params cannot be used with 
* runtime generated content. 
**********************************************************/

#define PROGRAM_NAME "simple"

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
#define JASMIN_LABEL(stream, x) JASMIN_WITH_NEWLINE(stream, SPACE x) // TODO - add ":""
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
#define INVOKE_PRINTLN_INT "invokevirtual java/io/PrintStream/println(I)V"
#define INVOKE_PRINTLN_BOOL "invokevirtual java/io/PrintStream/println(Z)V"
#define SUBROUTINE ".method public static "

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
	const static std::string SUB_SUBROUTINE;
	const static std::string MUL_SUBROUTINE;
	const static std::string DIV_SUBROUTINE;
	const static std::string LESS_THAN_SUBROUTINE;
	const static std::string EQ_TO_SUBROUTINE;

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
	* @return size of the environment (i.e. symbols table)
	*/
	int getEnvironmentSize() { return this->_environment.size(); }

	/**
	* Dynamically format an instruction into a jasmin instruction.
	* MACROS cannot be used on dynamic content
	*/
	void formatJasminInstruction(std::string& instruction);

	/**
	* Update the environment with the ID of the variable/expression and
	* the type of expression
	* @param ID of expression
	* @param expression type
	* @param onStack is true if the variable is added in the stack. Then the environment is updated.
	*				if false, then the environment is not updated, but the variable _lastAddedExpression
	*				is updated anyway. 
	*				In fact, there are cases when an integer value expression is executed, but not added in the stack
	*				but we may still want to keep track of it, in case this must be printed.
	*/
	void updateEnvironment(std::string* ID, AST::EXPRESSION_TYPE exprType, bool onStack);

	/**
	* Append a subroutine at the end of the given bytecode jasmin program
	* IF and ONLY IF the subroutine does not exist already.
	* @param bytecodeProgram
	*/
	void addSubroutine(AST::OP op, std::string& bytecodeProgram);

private:
	/**
	* Adding Print statement bytecode to output for 
	* a value which must be on top of the stack.
	*/
	void printInt(std::string& output);
	void printBool(std::string& output);

	boost::shared_ptr<AST::PROGRAM> _program;

	void addInitialJasminCode(std::string& output);
	void addInitialMainJasminCode(std::string& output);
	void addFinalMainJasminCode(std::string& output);
	void printLastStatement(std::string& output);

	/**
	* True if the subroutine has been already added to 
	* the Jasmin bytecode
	*/
	bool _addSubroutineEnabled;
	bool _subSubroutineEnabled;
	bool _mulSubroutineEnabled;
	bool _divSubroutineEnabled;
	bool _lessThanSubroutineEnabled;
	bool _eqToSubroutineEnabled;

	/**
	* The environment or symbol tables
	* is used to map "identifiers to their types and locations"
	* Appel A., Modern Compiler Implementation in C
	*/
	std::map< std::string, std::pair<int, AST::EXPRESSION_TYPE> > _environment;
	std::pair<std::string, AST::EXPRESSION_TYPE> _lastAddedExpression; 
};