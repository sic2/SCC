/***********************************************************
* PLDI - A Compiler to JVM Bytecode: 
*				scc - St andrews Case Compiler 
* Student: 100003610
* 
* This software is part of University coursework and 
* can be reused only for teaching/learning purposes. 
* The code of the scc cannot be copied or reused in any 
* circumstances, unless permission has been granted by the author.
***********************************************************/

#include "JVMByteCodeGenerator.h"
 #include "basicProgramTests.h"
#include "ADTProgramTests.h"

// A 2-steps process is used:
// - Encode the program in AST
// - Generate JVM bytecode from the AST
// The JVM bytecode is printed to file if successful. 
int main(int argc, char** argv)
{
	/**************
	* TEST PROGRAMS
	***************/
	// basicProgramTests test0;
	// JVMByteCodeGenerator byteCodeGen;
	// byteCodeGen.generateByteCode(test0.getTest3(), "XXX"); // FIXME - use filename for creating source file

	
	ADTProgramTests test0;
	JVMByteCodeGenerator byteCodeGen;
	byteCodeGen.generateByteCode(test0.getTest1(), "XXX");

	return 0;
}	