#include "CONSTR.h"

AST::CONSTR::CONSTR(std::string ID, std::vector< boost::shared_ptr<AST::TYPE> > types)
{
	this->_id = ID;
	this->_types = types;
}