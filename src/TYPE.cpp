#include "TYPE.h"

AST::TYPE::TYPE(PRIMITIVE_TYPE primitiveType)
{
	_primitiveType = primitiveType;
	_whichConstructor = TYPE_IS_PRIMITIVE;
}

AST::TYPE::TYPE(std::string ID, std::vector<TYPE> types)
{
	_id = ID;
	_types = types;
	_whichConstructor = TYPE_NOT_PRIMITIVE;
}