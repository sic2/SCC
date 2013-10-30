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
}

bool JVMByteCodeGenerator::generateByteCode(boost::shared_ptr<AST::PROGRAM> program, std::string outFileName) 
{	
	this->_program = program;

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
	return false; // FIXME
}

void JVMByteCodeGenerator::formatJasminInstruction(std::string& instruction)
{
	instruction = TAB + instruction + NEW_LINE;
}

void JVMByteCodeGenerator::printLastStatement(std::string& output)
{
	std::ostringstream convert; 
	convert << (_expressionsOnStack - 1); 
	
	switch(_lastExpression->type)
	{
		case AST::EXPR_INT:
			//output += "\tiload " + convert.str() + "\n"; 
			JASMIN_INSTR(output, INVOKE_PRINTLN_INT);
			break;
		case AST::EXPR_BOOL:
			//output += "\tiload " + convert.str() + "\n"; 
			JASMIN_INSTR(output, INVOKE_PRINTLN_BOOL);
			break;
		case AST::EXPR_STRING:
			printf("cannot print string yet\n");
			break;
		default:
			printf("ERROR: Printing last statement - type %d NOT SUPPORTED\n", _lastExpression->type);
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
	// Environment field
	JASMIN_DIRECTIVE(output, ".field public ENV I");
	
	JASMIN_DIRECTIVE(output, ".method public <init>()V");
	JASMIN_INSTR(output, "aload 0");
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