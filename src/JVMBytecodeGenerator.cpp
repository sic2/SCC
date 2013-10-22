#include "JVMByteCodeGenerator.h"

#define NEW_LINE "\n"
#define TAB "\t"
#define SPACE " "
#define JASMIN_DIRECTIVE(stream, x) stream += x NEW_LINE
#define JASMIN_LABEL(stream, x) stream += SPACE x NEW_LINE
#define JASMIN_INSTR(stream, x) stream += TAB x NEW_LINE


void JVMByteCodeGenerator::addInitialJasminCode(std::string& output)
{
	JASMIN_DIRECTIVE(output, ".class public simple");
	JASMIN_DIRECTIVE(output, ".super java/lang/Object");
	JASMIN_DIRECTIVE(output, ".method public <init>()V");
	JASMIN_INSTR(output, "aload_0");
	JASMIN_INSTR(output, "invokespecial java/lang/Object/<init>()V");
	JASMIN_INSTR(output, "return");
	JASMIN_DIRECTIVE(output, ".end method");
	JASMIN_DIRECTIVE(output, ".method public static main([Ljava/lang/String;)V");
}

void JVMByteCodeGenerator::addFinalJasminCode(std::string& output)
{
	JASMIN_INSTR(output, "return\t; return from main");
	JASMIN_DIRECTIVE(output, ".end method");
}

// Resources on jasmin:
// http://www.ceng.metu.edu.tr/courses/ceng444/link/f3jasmintutorial.html 
// http://jasmin.sourceforge.net/
bool JVMByteCodeGenerator::generateByteCode(std::string outFileName) 
{
	std::string output;
	addInitialJasminCode(output);

	// TODO
	// generate appropriate code

	addFinalJasminCode(output);
	
	if(DEBUG_MODE >= 1)
	{
		printf("JASMIN BYTECODE: \n\n%s\n\n  END JASMIN BYTECODE\n", output.c_str());
	}
	
	return false;
}


    
    
     
 
