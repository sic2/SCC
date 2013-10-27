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
	TYPE(PRIMITIVE_TYPE primitiveType);
	TYPE(std::string ID, std::vector<TYPE> types);

	virtual ~TYPE() {}

	/**
	* @return empty string if no id is set.
	*/
	std::string getID() { return _id; }

	/**
	* @return PRIMITIVE type
	*/
	PRIMITIVE_TYPE getType() { return this->_primitiveType; }

	/**
	* @return vector of types
	*/
	std::vector<TYPE> getTypes() { return this->_types; }

	/**
	* @return true if this type is a primitive
	*/
	bool typeIsPrimitive() { return _whichConstructor == TYPE_IS_PRIMITIVE; }

private:
	PRIMITIVE_TYPE _primitiveType;

	std::string _id;
	std::vector<TYPE> _types;

	int _whichConstructor;
};