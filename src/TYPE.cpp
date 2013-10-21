#include "AST.h"

AST::TYPE::TYPE(PRIMITIVE_TYPE primitiveType)
{
	_primitiveType = primitiveType;
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

