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
};