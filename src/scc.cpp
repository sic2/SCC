// scc - St andrews Case Compiler
#include "AST.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace AST;

int main(int argc, char** argv)
{

	// TODO
	// 1 - have hardcoded AST
	// 2 - generate JVM bytecode from AST

	/*
	* Define values
	*/
	uValue expr;
	expr.Integer = 10;
	
	ALT(new TYPE(TYPE_INT), new EXPR(EXPR_INT, expr));

	/*
	* Define test program
	*/
	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "+", new EXPR(EXPR_INT, expr)));

	PROGRAM(new EXPR(EXPR_BI_OP, new EXPR(EXPR_INT, expr), "&", new EXPR(EXPR_INT, expr)));
	
	return 0;
}


/* XXX - ADDITIONAL STUFF
	// FIXME - check if argument exist and if extension is .case
	string fileName = argv[1];
	string inputFile = "inputFiles/" + fileName;
	ifstream myfile (inputFile.c_str());

	// File input routine as in: http://www.cplusplus.com/doc/tutorial/files/
	string line;
	if (myfile.is_open())
	{
		while (getline(myfile,line))
		{ 
			cout << line << endl; // TODO - tokenise : http://www.cplusplus.com/reference/cstring/strtok/
		}
		myfile.close();
	}
	else
	{
		cout << "Unable to open file";
	} 
	*/
