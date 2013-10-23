#pragma once

#include "AST.h"

// TODO - use union and compress all constructors into one
class AST::TYPE
{
public:
	TYPE(PRIMITIVE_TYPE primitiveType);

	TYPE(std::string ID, AST::TYPE type);

	TYPE(std::string ID, std::vector<AST::TYPE> types);

  PRIMITIVE_TYPE getType() { return this->_primitiveType; } // FIXME - this will not work for types that do not have primitiveType

private:
	PRIMITIVE_TYPE _primitiveType;
	// uValue _uValue;
	std::string _id;
	std::vector<AST::TYPE> _types;

	int _whichConstructor;
};