#include "JVMByteCodeGenerator.h"

#include "PROGRAM.h"

#include <sstream>

// TODO - other subroutine names
const std::string JVMByteCodeGenerator::ADD_SUBROUTINE = "ADD_SUBROUTINE";

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
	
	switch(_lastExpressionAddedOnStack)
	{
		case AST::EXPR_INT:
			printInt(output, 10);
			break;
		default:
			printf("Printing last statement - type NOT SUPPORTED\n");
			break;
	} // end switch
}

void JVMByteCodeGenerator::printInt(std::string& output, int var)
{
	/*
	Assumption:
	The int variable to be printed is stored in #var
	using istore_var
	Example:
	 getstatic       #2; //Field java/lang/System.out:Ljava/io/PrintStream;
	 iload_0  
	 invokevirtual   #3; //Method java/io/PrintStream.println:(I)V
	*/
	JASMIN_INSTR(output, PRINT_STREAM);
	//output += "pop\n";
	//output += "\tiload_0\n"; // FIXME - 
	std::pair<int, AST::EXPRESSION_TYPE> secondPair = lastAdded->second;
	int test = secondPair.first;

	 std::ostringstream convert; 
	 convert << test;
	 JASMIN_WITHOUT_NEWLINE(output, "iload_");
	 output += convert.str() + "\n";
	JASMIN_INSTR(output, INVOKE_PRINTLN);
}

/**
* Private methods
*/
void JVMByteCodeGenerator::addInitialJasminCode(std::string& output)
{
	JASMIN_DIRECTIVE(output, ".class public simple"); // XXX - simple is the name of the program and must be saved as simple.j
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
		JASMIN_DIRECTIVE(bytecodeProgram, SUBROUTINE(ADD_SUBROUTINE));
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit stack 2");
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit locals 2");
		JASMIN_INSTR(bytecodeProgram, "iload_0");
		JASMIN_INSTR(bytecodeProgram, "iload_0");
		JASMIN_INSTR(bytecodeProgram, "iadd");
		JASMIN_INSTR(bytecodeProgram, "ireturn");
		JASMIN_DIRECTIVE(bytecodeProgram, ".end method");
		bytecodeProgram += "\n";
	}
	_addSubroutineEnabled = true;
}