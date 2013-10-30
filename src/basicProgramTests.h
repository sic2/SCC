#pragma once

#include "baseProgramTest.h"

class basicProgramTests : public baseProgramTest
{
public:
	basicProgramTests() : baseProgramTest() {}
	virtual ~basicProgramTests() {}

	boost::shared_ptr<AST::PROGRAM> getTest0();
	boost::shared_ptr<AST::PROGRAM> getTest1();
	boost::shared_ptr<AST::PROGRAM> getTest2();
	boost::shared_ptr<AST::PROGRAM> getTest3();
	boost::shared_ptr<AST::PROGRAM> getTest4();
	boost::shared_ptr<AST::PROGRAM> getTest5();
	boost::shared_ptr<AST::PROGRAM> getTest6();
	boost::shared_ptr<AST::PROGRAM> getTest7();
	boost::shared_ptr<AST::PROGRAM> getTest8();
	boost::shared_ptr<AST::PROGRAM> getTest9();
};