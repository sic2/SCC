#include "AST.h"

AST::TYPE::TYPE(PRIMITIVE_TYPE primitiveType, uValue value)
{
	_primitiveType = primitiveType;
	_uValue = value;
	_whichConstructor = 0;
}

AST::TYPE::TYPE(std::string ID, AST::TYPE type)
{
	_id = ID;
	_types.push_back(type);
	_whichConstructor = 1;
} 

AST::TYPE::TYPE(std::string ID, std::vector<AST::TYPE> types)
{
	_id = ID;
	_types = types;
	_whichConstructor = 2;
}

bool AST::TYPE::getValue(PRIMITIVE_TYPE* primitiveType, uValue* value)
{
	if (_whichConstructor != 0)
	{
		return false;
	}
	*primitiveType = _primitiveType;
	*value = _uValue;
	
	return true;
}