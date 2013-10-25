#include "JVMByteCodeGenerator.h"

#include "PROGRAM.h"

#include <sstream>

// MACROS
#define JASMIN_DIRECTIVE_progr(stream, x, progr_Name) stream += x progr_Name NEW_LINE

JVMByteCodeGenerator::JVMByteCodeGenerator(boost::shared_ptr<AST::PROGRAM> program)
{
	_program = program;
}

// Resources on jasmin:
// http://www.ceng.metu.edu.tr/courses/ceng444/link/f3jasmintutorial.html 
// http://jasmin.sourceforge.net/
bool JVMByteCodeGenerator::generateByteCode(std::string outFileName) 
{
	std::string jasminProgram;
	std::string mainMethod;
	addInitialJasminCode(jasminProgram);
	addInitialMainJasminCode(mainMethod);

	// Traversing the AST and generate program
	(*_program).generateByteCode(this, jasminProgram, mainMethod);	

	printLastStatement(mainMethod);
	addFinalMainJasminCode(mainMethod);

	jasminProgram += mainMethod; // Stich the main method with the rest of the program

	if(DEBUG_MODE >= 1)
	{
		printf("JASMIN BYTECODE: \n\n%s\n\nEND JASMIN BYTECODE\n", jasminProgram.c_str());
	}

	return false;
}

void JVMByteCodeGenerator::formatJasminInstruction(std::string& instruction)
{
	instruction = TAB + instruction + NEW_LINE;
}


void JVMByteCodeGenerator::printLastStatement(std::string& output)
{
	if (_lastAddedExpression.first.compare("") != 0)
	{
		std::ostringstream convert; 
		convert << _environment.find(_lastAddedExpression.first)->second.first;
		output += "\t iload_" + convert.str() + "\n";
	}
	switch(_lastAddedExpression.second)
	{
		case AST::EXPR_INT:
			JASMIN_INSTR(output, INVOKE_PRINTLN_INT);
			break;
		case AST::EXPR_BOOL:
			JASMIN_INSTR(output, INVOKE_PRINTLN_BOOL);
			break;
		default:
			printf("Printing last statement - type NOT SUPPORTED\n");
			break;
	} // end switch
}

/**
* Private methods
*/
void JVMByteCodeGenerator::addInitialJasminCode(std::string& output)
{
	JASMIN_DIRECTIVE_progr(output, ".class public ", PROGRAM_NAME); // XXX - simple is the name of the program and must be saved as simple.j
	JASMIN_DIRECTIVE(output, ".super java/lang/Object");
	JASMIN_DIRECTIVE(output, ".method public <init>()V");
	JASMIN_INSTR(output, "aload_0");
	JASMIN_INSTR(output, INVOKE_SPECIAL_INT);
	JASMIN_INSTR(output, "return");
	JASMIN_DIRECTIVE(output, ".end method");
	output += "\n";
}

void JVMByteCodeGenerator::addInitialMainJasminCode(std::string& output)
{
	JASMIN_DIRECTIVE(output, ".method public static main([Ljava/lang/String;)V");	
	JASMIN_STACK(output, 5);
	JASMIN_LOCALS(output, 100);
}
void JVMByteCodeGenerator::addFinalMainJasminCode(std::string& output)
{
	JASMIN_INSTR(output, "return\t; return from main"); 
	JASMIN_DIRECTIVE(output, ".end method");
}

bool JVMByteCodeGenerator::addSubroutine(std::string subroutine)
{

	const bool is_in = _subRoutines.find(subroutine) != _subRoutines.end();
	if (!is_in)
	{
		_subRoutines.insert(subroutine);
	}
	return !is_in;
}

void JVMByteCodeGenerator::updateEnvironment(std::string* ID, AST::EXPRESSION_TYPE exprType, bool onStack)
{
	
	int varIndex = _environment.size();
	_lastAddedExpression = std::make_pair<std::string, AST::EXPRESSION_TYPE> (*ID, exprType);
	if (onStack)
	{
		_environment.insert(std::make_pair<std::string, std::pair<int, AST::EXPRESSION_TYPE> > 
								(*ID, std::make_pair<int, AST::EXPRESSION_TYPE> (varIndex, exprType))); // XXX - not sure if copied or passed by reference
	}
}