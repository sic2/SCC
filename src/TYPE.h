#pragma once

#include "AST.h"

#define TYPE_IS_PRIMITIVE 0
#define TYPE_NOT_PRIMITIVE 1

/*
* TODO
* use union (i.e. variant) instead of having two constructors
*/
class AST::TYPE
{
public:
	TYPE(std::string ID, std::vector< boost::shared_ptr<TYPE> > types);

	virtual ~TYPE() {}

	/**
	* @return empty string if no id is set.
	*/
	std::string getID() { return _id; }

	/**
	* @return vector of types
	*/
	std::vector< boost::shared_ptr<TYPE> > getTypes() { return this->_types; }

	bool isPrimitive();

private:
	std::string _id;
	std::vector< boost::shared_ptr<TYPE> > _types;
};