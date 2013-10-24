#include "JVMByteCodeGenerator.h"

#include "PROGRAM.h"

#include <sstream>

#define JASMIN_DIRECTIVE_progr(stream, x, progr_Name) stream += x progr_Name NEW_LINE

// TODO - other subroutine names
const std::string JVMByteCodeGenerator::ADD_SUBROUTINE = "ADD_SUBROUTINE";
const std::string JVMByteCodeGenerator::SUB_SUBROUTINE = "SUB_SUBROUTINE";

JVMByteCodeGenerator::JVMByteCodeGenerator(boost::shared_ptr<AST::PROGRAM> program)
{
	_program = program;

	_addSubroutineEnabled = false;
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

	// TODO - print last statement on stack
	// pop
	// appropriate print [ SHIT ]

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
	
	switch(_lastAddedExpression.second)
	{
		case AST::EXPR_INT:
			printInt(output);
			break;
		default:
			printf("Printing last statement - type NOT SUPPORTED\n");
			break;
	} // end switch
}

void JVMByteCodeGenerator::printInt(std::string& output)
{	
	if (_lastAddedExpression.first.compare("") != 0)
	{
		std::ostringstream convert; 
		convert << _environment.find(_lastAddedExpression.first)->second.first;
		output += "iload_" + convert.str() + "\n";
	}
	JASMIN_INSTR(output, INVOKE_PRINTLN_INT);
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

void JVMByteCodeGenerator::addSubroutine(std::string& bytecodeProgram)
{
	if (!_addSubroutineEnabled)
	{
		bytecodeProgram += SUBROUTINE + ADD_SUBROUTINE + "(II)I" + "\n";
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit stack 2");
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit locals 2");
		JASMIN_INSTR(bytecodeProgram, "iload_0");
		JASMIN_INSTR(bytecodeProgram, "iload_1");
		JASMIN_INSTR(bytecodeProgram, "iadd");
		JASMIN_INSTR(bytecodeProgram, "ireturn");
		JASMIN_DIRECTIVE(bytecodeProgram, ".end method");
		bytecodeProgram += "\n";
	}
	_addSubroutineEnabled = true;
}

void JVMByteCodeGenerator::subSubroutine(std::string& bytecodeProgram)
{
	if (!_subSubroutineEnabled)
	{
		bytecodeProgram += SUBROUTINE + SUB_SUBROUTINE + "(II)I" + "\n";
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit stack 2");
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit locals 2");
		JASMIN_INSTR(bytecodeProgram, "iload_0");  // FIXME - merge with addSubroutine
		JASMIN_INSTR(bytecodeProgram, "iload_1");
		JASMIN_INSTR(bytecodeProgram, "isub");
		JASMIN_INSTR(bytecodeProgram, "ireturn");
		JASMIN_DIRECTIVE(bytecodeProgram, ".end method");
		bytecodeProgram += "\n";
	}
	_subSubroutineEnabled = true;
}

void JVMByteCodeGenerator::updateEnvironment(std::string* ID, AST::EXPRESSION_TYPE exprType, bool onStack)
{
	
	int varIndex = _environment.size();
	printf("var index is %d\n", varIndex);
	_lastAddedExpression = std::make_pair<std::string, AST::EXPRESSION_TYPE> (*ID, exprType);
	if (onStack)
	{
		_environment.insert(std::make_pair<std::string, std::pair<int, AST::EXPRESSION_TYPE> > 
								(*ID, std::make_pair<int, AST::EXPRESSION_TYPE> (varIndex, exprType))); // XXX - not sure if copied or passed by reference
	}
}