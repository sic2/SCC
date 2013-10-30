#include "Helper.h"

#include <iostream>
#include <fstream>
#include <sstream>

Helper Helper::sInstance;

Helper& Helper::instance()
{
	return sInstance;
}

void Helper::saveToFile(std::string fileName, std::string data)
{
	std::ofstream file;
	file.open (fileName.c_str());
	file << data;
	file.close();
}

std::string Helper::integerToString(int value)
{
	std::ostringstream convert; 
	convert << value;
	return convert.str();
}