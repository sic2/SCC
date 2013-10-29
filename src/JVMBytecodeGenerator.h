#pragma once

#include "AST.h"
#include <map>
#include <set>

/**********************************************************
* MACROS
* Publicly accessible since they are
* useful to other parts of the compiler to 
* create various parts of the jasmin bytecode.
* However, MACROS accepting params cannot be used with 
* runtime generated content. 
**********************************************************/

#define PROGRAM_NAME "simple"  // TODO - define name of program from main args

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

/**
* This class is used to keep track of a current state
* of a given executed instruction.
*/
class Expression_Info
{
public:
	inline Expression_Info(AST::EXPRESSION_TYPE type, std::string ID, int locationInStack)
	{
		this->type = type;
		this->ID = ID;
		this->locationInStack = locationInStack;
	}

	virtual ~Expression_Info() {}

	AST::EXPRESSION_TYPE type;
	std::string ID;
	int locationInStack;
};

class JVMByteCodeGenerator
{
public:
	JVMByteCodeGenerator();
	virtual ~JVMByteCodeGenerator() {}

	/**
	* METHOD NOT FINISHED
	*/
	void cleanup();

	/**
	* Generates JVM bytecode and output to file
	* @param outFileName 
	* @return 	True if bytecode was generate successfully.
	*			False otherwise.
	*/
	bool generateByteCode(boost::shared_ptr<AST::PROGRAM> program, std::string outFileName);

	/**
	* @return size of the environment (i.e. symbols table)
	*/
	int getEnvironmentSize() { return _expressionsOnStack; }

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
	*				if false, then the environment is not updated, but the variable _lastExpression
	*				is updated anyway. 
	*				In fact, there are cases when an integer value expression is executed, but not added in the stack
	*				but we may still want to keep track of it, in case this must be printed.
	*/
	void updateEnvironment(std::string* ID, AST::EXPRESSION_TYPE exprType, bool onStack);

	/*
	* Typedef getters and setters for the environment
	*/
	void addTypedef(std::string typeID, AST::Expr_Typedef typeDefinition);

	bool typeIsDefined(std::string typeID);

	AST::Expr_Typedef getTypeDef(std::string typeID);

	/**
	* Add a subroutine to the environment (subroutines table)
	* IF and ONLY IF the subroutine does not exist already.
	* @param subroutine name
	* @return true if subroutine was added, false if it was already
	*			in the environment
	*/
	bool addSubroutine(std::string subroutine);

	int nextLabel();

	int currentLabel();

	/**
	* Generate byte code for Algebric data types.
	* The code is generate only once for all the program.
	*/
	void addGenericClassForADTs() { this->_genericClassForADTsEnabled = true; }

	/**
	* Adds a reference to a generic object to the environment
	*/
	void addNewGenericObject(std::string str, int labelIndex);

	/*
	* Return the last scheduled expression
	*/
	Expression_Info* getLastExpression() { return this->_lastExpression; }

private:
	boost::shared_ptr<AST::PROGRAM> _program;

	void addInitialJasminCode(std::string& output);
	void addInitialMainJasminCode(std::string& output);
	void addFinalMainJasminCode(std::string& output);
	void printLastStatement(std::string& output);

	/**
	* The environment or symbol tables
	* is used to map "identifiers to their types and locations"
	* Appel A., Modern Compiler Implementation in C
	*/

	/*
	* Keep track of ids -> type
	*/
	std::map<std::string, AST::Expr_Typedef> _typeDefinitions;

	/*
	* Keep track of created objects and registers
	* used to store instances
	*/
	std::map<std::string, int> _objects; // XXX - updated but never used

	int _expressionsOnStack;
	Expression_Info* _lastExpression;

	/**
	* Set containing the subroutines names of all subroutines 
	* used so far.
	*/
	std::set<std::string> _subRoutines;

	int _numberLabels;
	bool _genericClassForADTsEnabled;
};