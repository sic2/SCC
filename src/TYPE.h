#pragma once

#include "AST.h"

#define TYPE_IS_PRIMITIVE 0
#define TYPE_NOT_PRIMITIVE 1

class AST::TYPE
{
public:
	TYPE(PRIMITIVE_TYPE primitiveType);
	TYPE(std::string ID, std::vector<TYPE> types);

	virtual ~TYPE() {}

	/**
	* @return empty string if no id is set.
	*/
	std::string getID();

	/**
	* @return set of PRIMITIVE types
	*/
	PRIMITIVE_TYPE getType() { return this->_primitiveType; }

	/**
	*
	*/
	std::vector<TYPE> getTypes() { return this->_types; }

	/**
	*
	*/
	bool typeIsPrimitive() { return _whichConstructor == TYPE_IS_PRIMITIVE; }

private:
	PRIMITIVE_TYPE _primitiveType;

	std::string _id;
	std::vector<TYPE> _types;

	int _whichConstructor;
};