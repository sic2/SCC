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
	// Suite0
	basicProgramTests suite0;
	JVMByteCodeGenerator byteCodeGen;
	byteCodeGen.generateByteCode(suite0.getTest0(), "s0Test0");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest1(), "s0Test1");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest2(), "s0Test2");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest3(), "s0Test3");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest4(), "s0Test4");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest5(), "s0Test5");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest6(), "s0Test6");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest7(), "s0Test7");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite0.getTest9(), "s0Test9");
	
	// Suite1
	ADTProgramTests suite1;
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite1.getTest0(), "s1Test0");
	byteCodeGen.cleanup();
	byteCodeGen.generateByteCode(suite1.getTest1(), "s1Test1");
	return 0;
}	