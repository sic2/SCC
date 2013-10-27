#pragma once

#include "baseProgramTest.h"

/**
* Program tests about ADT - Algebric Data Types
*/
class ADTProgramTests : public baseProgramTest
{
public:
	ADTProgramTests() : baseProgramTest() {}
	virtual ~ADTProgramTests() {}

	boost::shared_ptr<AST::PROGRAM> getTest0();
	//boost::shared_ptr<AST::PROGRAM> getTest1();
	//boost::shared_ptr<AST::PROGRAM> getTest2();
};