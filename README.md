# PLDI - A Compiler to JVM Bytecode

Student: 100003610

# Case Language

	<program> ::= <expr> "."

	<expr> ::= <int>
			| <bool> 
			| <string> 
			| <id> <expr>* 
			| “case” <expr> “of” <alts> 
			| “for” <id> "in" <expr> “do” <expr> 
			| <expr> <op> <expr> 
			| “{” <expr> “;” ... ";" <expr> “}” 
			| <typedef> | <id> "::" <type> "=" <expr>


	<alts> ::= <alt> "|" ... "|" <alt> 
	<alt> ::= <type>+ "->" <expr>

	<op> ::= “+” | “*” | “-” | “div”| “<”| “==” | ".." | "or" | "and"

	<typedef> ::= "type" <id> <args> "=" <constr> <constrs> 
	<constrs> ::= ( "|" <constr> ) * 
	<constr> ::= <id> <type>*

	<type> ::= "int" | "bool" | "string" | "(" <id> <type>* ")"

## Language decisions

# Requirements 

Tested Targets: 

	i686-apple-darwin10

Other:

	Boost 1.54
	Location assumed: ~/usr/include/boost/

	gcc 4.2.1
