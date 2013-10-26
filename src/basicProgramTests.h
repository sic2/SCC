#pragma once

#include "baseProgramTest.h"

class basicProgramTests : baseProgramTest
{
public:
	basicProgramTests() : baseProgramTest() {}
	virtual ~basicProgramTests() {}

	boost::shared_ptr<AST::PROGRAM> getIt();
};