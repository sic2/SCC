#pragma once

#include "AST.h"

class AST::ALT
{
public:
	/*
	* @param condition is the condition value for cases in switch statements
	* A condition must be either an integer, a bool or a string // FIXME
	*/
	ALT(boost::shared_ptr<AST::EXPR>* condition, boost::shared_ptr<AST::EXPR>* expr);
	virtual ~ALT() {}

	boost::shared_ptr<AST::EXPR>* getCondition(); 
	boost::shared_ptr<AST::EXPR>* getEXPR();

private:
	boost::shared_ptr<AST::EXPR>* _condition;
	boost::shared_ptr<AST::EXPR>* _expr;
};