#pragma once

#include "AST.h"
#include "TYPE.h"

class AST::CONSTR
{
public:
	CONSTR(std::string ID, std::vector< boost::shared_ptr<AST::TYPE> > types);
	virtual ~CONSTR() {}

	std::string getID() { return this->_id; }
	std::vector< boost::shared_ptr<AST::TYPE> > getTypes() { return this->_types; }

private:
	std::string _id;
	std::vector< boost::shared_ptr<AST::TYPE> > _types;	
};