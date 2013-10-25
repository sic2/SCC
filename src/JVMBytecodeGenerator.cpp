#include "JVMByteCodeGenerator.h"

#include "PROGRAM.h"

#include <sstream>

// MACROS
#define JASMIN_DIRECTIVE_progr(stream, x, progr_Name) stream += x progr_Name NEW_LINE

// String literals
#define INVALID_OPERATION "INVALID_OPERATION"

// TODO - other subroutine names
const std::string JVMByteCodeGenerator::ADD_SUBROUTINE = "ADD_SUBROUTINE";
const std::string JVMByteCodeGenerator::SUB_SUBROUTINE = "SUB_SUBROUTINE";
const std::string JVMByteCodeGenerator::MUL_SUBROUTINE = "MUL_SUBROUTINE";
const std::string JVMByteCodeGenerator::DIV_SUBROUTINE = "DIV_SUBROUTINE";
const std::string JVMByteCodeGenerator::LESS_THAN_SUBROUTINE = "LESS_THAN_SUBROUTINE";
const std::string JVMByteCodeGenerator::EQ_TO_SUBROUTINE = "EQ_TO_SUBROUTINE";

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
		case AST::EXPR_BOOL:
			printBool(output);
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

void JVMByteCodeGenerator::printBool(std::string& output)
{	
	if (_lastAddedExpression.first.compare("") != 0)
	{
		std::ostringstream convert; 
		convert << _environment.find(_lastAddedExpression.first)->second.first;
		output += "iload_" + convert.str() + "\n";
	}
	JASMIN_INSTR(output, INVOKE_PRINTLN_BOOL);
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

// TODO - pass operands types too
void JVMByteCodeGenerator::addSubroutine(AST::OP op, std::string& bytecodeProgram)
{
	bool subroutineNeeded = false;
	std::string operation = INVALID_OPERATION;
	std::string nameSubroutine;
	AST::PRIMITIVE_TYPE returnType = AST::TYPE_INT; // default return type for subroutine
	switch(op)
	{
		case AST::OP_ADDITION:
			if (!_addSubroutineEnabled)
			{
				subroutineNeeded = true;
				_addSubroutineEnabled = true;
				operation = "iadd";
				nameSubroutine = ADD_SUBROUTINE;
			}
		break;
		case AST::OP_SUBTRACTION:
			if (!_subSubroutineEnabled)
			{
				subroutineNeeded = true;
				_subSubroutineEnabled = true;
				operation = "isub";
				nameSubroutine = SUB_SUBROUTINE;
			}
		break;
		case AST::OP_MULTIPLICATION:
			if (!_mulSubroutineEnabled)
			{
				subroutineNeeded = true;
				_mulSubroutineEnabled = true;
				operation = "imul";
				nameSubroutine = MUL_SUBROUTINE;
			}
		break;
		case AST::OP_DIVISION:
			if (!_divSubroutineEnabled)
			{
				subroutineNeeded = true;
				_divSubroutineEnabled = true;
				operation = "idiv";
				nameSubroutine = DIV_SUBROUTINE;
			}
		break;
		case AST::OP_EQUALITY:
			if (!_eqToSubroutineEnabled)
			{
				subroutineNeeded = true;
				_eqToSubroutineEnabled = true;
				nameSubroutine = EQ_TO_SUBROUTINE;
				returnType = AST::TYPE_BOOL;
			}
			break;
		case AST::OP_LESS:
			if (!_lessThanSubroutineEnabled)
			{
				subroutineNeeded = true;
				_lessThanSubroutineEnabled = true;
				nameSubroutine = LESS_THAN_SUBROUTINE;
				returnType = AST::TYPE_BOOL;
			}
			break;
		case AST::OP_OR:
		case AST::OP_AND:
		case AST::OP_RANGE:
		default:
			printf("operation not supported\n");
		break;
	} // end switch

	// Construct subroutine params
	if (subroutineNeeded)
	{
		switch(returnType)
		{
			case AST::TYPE_INT:
				bytecodeProgram += SUBROUTINE + nameSubroutine + "(II)I" + "\n"; 
			break;
			case AST::TYPE_BOOL:
				bytecodeProgram += SUBROUTINE + nameSubroutine + "(II)Z" + "\n"; 
			break;
			case AST::TYPE_STRING:
				printf("Return type for subroutine not supported\n");
			default: 
			break;
		}

		// Initialise subroutine - common for all types of subroutines taking 2 arguments
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit stack 2");
		JASMIN_DIRECTIVE(bytecodeProgram, ".limit locals 2");
		JASMIN_INSTR(bytecodeProgram, "iload_0");
		JASMIN_INSTR(bytecodeProgram, "iload_1");

		if (operation.compare(INVALID_OPERATION) != 0)
		{
			bytecodeProgram += "\t" + operation + "\n";
		}
		else if (nameSubroutine.compare(LESS_THAN_SUBROUTINE) == 0 ||
				nameSubroutine.compare(EQ_TO_SUBROUTINE) == 0)
		{	
			// Apply the correct branch comparison
			if (nameSubroutine.compare(LESS_THAN_SUBROUTINE) == 0)
			{
				JASMIN_INSTR(bytecodeProgram, "if_icmpge Label_NT");
			} 
			else if (nameSubroutine.compare(EQ_TO_SUBROUTINE) == 0)
			{
				JASMIN_INSTR(bytecodeProgram, "if_icmpne Label_NT");
			}
			
			// Branch flow is the same for all routines above
			JASMIN_INSTR(bytecodeProgram, "iconst_1");
			JASMIN_INSTR(bytecodeProgram, "goto Label_T");
			JASMIN_LABEL(bytecodeProgram, "Label_NT:"); 
			JASMIN_INSTR(bytecodeProgram, "iconst_0");
			JASMIN_LABEL(bytecodeProgram, "Label_T:"); 
		}
		else
		{
			printf("operation not supported yet\n");
		}

		// Return from subroutine - common for all types of subroutines
		JASMIN_INSTR(bytecodeProgram, "ireturn");
		JASMIN_DIRECTIVE(bytecodeProgram, ".end method");
		bytecodeProgram += "\n";
	}
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