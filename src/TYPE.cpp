#include "TYPE.h"

AST::TYPE::TYPE(std::string ID, std::vector<TYPE> types)
{
	_id = ID;
	// int, bool and string are keywords, so ignore types
	if (ID.compare("int") != 0 &&
		ID.compare("bool") != 0 &&
		ID.compare("string") != 0)
	{
		_types = types;
	}
}