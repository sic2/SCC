#include "JVMByteCodeGenerator.h"
#include "ADTByteCode.h"
#include "PROGRAM.h"

#include "Helper.h"

#include <sstream>

// MACROS
#define JASMIN_DIRECTIVE_progr(stream, x, progr_Name) stream += x progr_Name NEW_LINE

#define TARGETS "targets/"

JVMByteCodeGenerator::JVMByteCodeGenerator()
{
	cleanup();	
}

void JVMByteCodeGenerator::cleanup()
{
	_numberLabels = -1;
	_expressionsOnStack = 0;
	_genericClassForADTsEnabled = false;
	
	_lastExpression = new Expression_Info(AST::EXPR_UNDEFINED, std::string(" "), 0);

	_typeDefinitions.clear();
	_objects.clear();
	_subRoutines.clear();
	_tmpObjects.clear();
}

void JVMByteCodeGenerator::generateByteCode(boost::shared_ptr<AST::PROGRAM> program, std::string outFileName) 
{	
	this->_program = program;
	this->_outFileName = outFileName;

	std::string jasminProgram;
	std::string mainMethod;
	addInitialJasminCode(jasminProgram);
	addInitialMainJasminCode(mainMethod);

	// Traversing the AST and generate program
	_program->generateByteCode(this, jasminProgram, mainMethod);	

	printLastStatement(mainMethod);
	addFinalMainJasminCode(mainMethod);

	jasminProgram += mainMethod; // Stich the main method with the rest of the program

	if(DEBUG_MODE >= 1)
	{
		printf("JASMIN BYTECODE: \n\n%s\n\nEND JASMIN BYTECODE\n\n", jasminProgram.c_str());
	}

	if (_genericClassForADTsEnabled)
	{
		std::string adtByteCode = ADTByteCode::getByteCode();
		std::string outputFileNameADT = TARGETS + std::string("ADTByteCode.j");
		Helper::instance().saveToFile(outputFileNameADT, adtByteCode);
	}
	
	outFileName = TARGETS + outFileName + ".j";
	Helper::instance().saveToFile(outFileName, jasminProgram);
}

void JVMByteCodeGenerator::formatJasminInstruction(std::string& instruction)
{
	instruction = TAB + instruction + NEW_LINE;
}

void JVMByteCodeGenerator::printLastStatement(std::string& output)
{
	switch(_lastExpression->type)
	{
		case AST::EXPR_INT:
			JASMIN_INSTR(output, INVOKE_PRINTLN_INT);
			break;
		case AST::EXPR_BOOL:
			JASMIN_INSTR(output, INVOKE_PRINTLN_BOOL);
			break;
		case AST::EXPR_STRING:
			JASMIN_INSTR(output, INVOKE_PRINTLN_STR);
			break;
		default:
			printf("ERROR: Printing last statement - type %d NOT SUPPORTED\n", _lastExpression->type);
			break;
	} // end switch
	
	/*
	 [ NOTE ] The code below is an alternative solution that
	 was tried to print the last statement.
	 This solution works by keeping track of the type of the data
	 on top of the stack and then dynamically choose what method 
	 to use for printing. 
	 However, I did not manage to get this working, but this will be left
	 here for reference.
	 **************************************************************
	 
	 output += "getstatic " + _outFileName + "/ENV I \n";
	 output += "iconst_0 \n";
	 output += "if_icmpne NT_1 \n";
	 output += "dup \n";
	 JASMIN_INSTR(output, "getstatic java/lang/System/out Ljava/io/PrintStream;");
	 output += "swap \n";
	 output += std::string(INVOKE_PRINTLN_INT) + "\n";
	 
	 output += "NT_1: \n";
	 output += "getstatic " + _outFileName + "/ENV I \n";
	 output += "iconst_1 \n";
	 output += "if_icmpne NT_2 \n";
	 output += "dup \n";
	 JASMIN_INSTR(output, "getstatic java/lang/System/out Ljava/io/PrintStream;");
	 output += "swap \n";
	 output += std::string(INVOKE_PRINTLN_BOOL) + "\n";
	 
	 output += "NT_2: \n";
	 output += "getstatic " + _outFileName + "/ENV I \n";
	 output += "iconst_2 \n";
	 output += "if_icmpne NT_3 \n";
	 output += "dup \n";
	 JASMIN_INSTR(output, "getstatic java/lang/System/out Ljava/io/PrintStream;");
	 output += "swap \n";
	 output += std::string(INVOKE_PRINTLN_STR) + "\n";
	 
	 output += "NT_3: \n";
	 output += "getstatic " + _outFileName + "/ENV I \n";
	 output += "iconst_3 \n";
	 output += "if_icmpne NT_4 \n";
	 output += "dup \n";
	 JASMIN_INSTR(output, "getstatic java/lang/System/out Ljava/io/PrintStream;");
	 output += "swap \n";
	 output += std::string(INVOKE_PRINTLN_INT) + "\n";
	 output += "NT_4: \n";
	 */
}

/**
* Private methods
*/
void JVMByteCodeGenerator::addInitialJasminCode(std::string& output)
{
	output += ".class public " + _outFileName + "\n";
	JASMIN_DIRECTIVE(output, ".super java/lang/Object");
	// Environment field
	// Keep track of the type of the last element on stack
	JASMIN_DIRECTIVE(output, ".field static public ENV I"); 
	
	JASMIN_DIRECTIVE(output, ".method public <init>()V");
	JASMIN_INSTR(output, "aload 0");
	JASMIN_INSTR(output, INVOKE_SPECIAL_INT);
	JASMIN_INSTR(output, "return");
	JASMIN_DIRECTIVE(output, ".end method");
	
	output += "\n";
}

std::string JVMByteCodeGenerator::getIntByteCode(int Integer)
{
	std::string retval = "iconst_";
	if (Integer == -1)
	{
		return "iconst_m1";
	}
	else if (Integer < -1 || Integer > 5)
	{
		retval = "bipush ";
	}
	
	return retval + Helper::instance().integerToString(Integer);
}

void JVMByteCodeGenerator::updateProgramEnv(std::string& output, int type)
{
	/*
	 [ NOTE ] The following bytecode is disabled since I was not able
	 to keep track of the environment within the program.
	 However, I am leaving it for reference.
	 
	output += "; update ENV \n";
	output += getIntByteCode(type) + "\n";
	output += "putstatic " + _outFileName + "/ENV I \n";
	 */
}

void JVMByteCodeGenerator::addInitialMainJasminCode(std::string& output)
{
	JASMIN_DIRECTIVE(output, ".method public static main([Ljava/lang/String;)V");	
	JASMIN_STACK(output, 10);
	JASMIN_LOCALS(output, 100);
	JASMIN_INSTR(output, "getstatic java/lang/System/out Ljava/io/PrintStream;");
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
	_lastExpression->type = exprType;
	_lastExpression->ID = *ID;
	if (onStack)
	{
		_lastExpression->locationInStack = _expressionsOnStack;
		_expressionsOnStack++;
	}
}

void JVMByteCodeGenerator::addTypedef(std::string typeID, AST::Expr_Typedef typeDefinition)
{
	this->_typeDefinitions.insert(std::make_pair<std::string, AST::Expr_Typedef> (typeID, typeDefinition));
}

bool JVMByteCodeGenerator::typeIsDefined(std::string typeID)
{
	return _typeDefinitions.find(typeID) != _typeDefinitions.end();
}

AST::Expr_Typedef JVMByteCodeGenerator::getTypeDef(std::string typeID)
{
	return _typeDefinitions.find(typeID)->second;
}

bool JVMByteCodeGenerator::objIsDefined(std::string ID)
{
	return _objects.find(ID) != _objects.end();
}

std::pair<std::string, int> JVMByteCodeGenerator::getObj(std::string ID)
{
	return _objects.find(ID)->second;
}

void JVMByteCodeGenerator::addNewGenericObject(std::string str, int labelIndex, std::string typeID)
{
	_objects.insert(std::make_pair<std::string, std::pair<std::string, int> >
					(str, std::make_pair<std::string, int>(typeID, labelIndex)));
}


std::pair<std::string, int> JVMByteCodeGenerator::getTmpObj(std::string ID)
{
	return _tmpObjects.find(ID)->second;
}

void JVMByteCodeGenerator::addNewTmpObject(std::string str, int labelIndex, std::string typeID)
{
	_tmpObjects.insert(std::make_pair<std::string, std::pair<std::string, int> >
					(str, std::make_pair<std::string, int>(typeID, labelIndex)));
}

int JVMByteCodeGenerator::nextLabel()
{
	this->_numberLabels++;
	return this->_numberLabels;
}

int JVMByteCodeGenerator::currentLabel()
{
	return this->_numberLabels;
}