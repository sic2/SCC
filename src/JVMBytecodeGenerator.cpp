#include "JVMByteCodeGenerator.h"

#include <sstream>

/**********
* MACROS
**********/
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

// Resources on jasmin:
// http://www.ceng.metu.edu.tr/courses/ceng444/link/f3jasmintutorial.html 
// http://jasmin.sourceforge.net/
bool JVMByteCodeGenerator::generateByteCode(std::string outFileName) 
{
	std::string output;
	addInitialJasminCode(output);
	JASMIN_STACK(output, 5);
	JASMIN_LOCALS(output, 100);

	// TODO
	// generate appropriate code
	// examine the _program
	//JASMIN_INSTR(output, "iconst_1");
	//JASMIN_INSTR(output, "istore_0");
	_program.generateByteCode(output);	

	addFinalJasminCode(output);
	
	if(DEBUG_MODE >= 1)
	{
		printf("JASMIN BYTECODE: \n\n%s\n\nEND JASMIN BYTECODE\n", output.c_str());
	}
	
	return false;
}

void JVMByteCodeGenerator::formatJasminInstruction(std::string& instruction)
{
	instruction = TAB + instruction + NEW_LINE;
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
	std::ostringstream convert; 
	convert << var;
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
	JASMIN_DIRECTIVE(output, ".method public static main([Ljava/lang/String;)V");
}

void JVMByteCodeGenerator::addFinalJasminCode(std::string& output)
{
	JASMIN_INSTR(output, "return\t; return from main"); 
	JASMIN_DIRECTIVE(output, ".end method");
}

    