#include "TYPE.h"

AST::TYPE::TYPE(std::string ID, std::vector< boost::shared_ptr<TYPE> > types)
{
	_id = ID;
	// int, bool and string are keywords, so ignore types
	if (!isPrimitive())
	{
		_types = types;
	}
}

bool AST::TYPE::isPrimitive()
{
	return _id.compare("int") == 0 ||
		_id.compare("bool") == 0 ||
		_id.compare("string") == 0;
}