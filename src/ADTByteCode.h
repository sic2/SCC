/**
* ADTByteCode specify a generic class for ADT
* Algebric data types
*/
class ADTByteCode
{
public:
	inline static std::string getByteCode()
	{
		std::string byteCode = ".source ADTByteCode.j \n";
		byteCode += ".class public ADTByteCode \n";
		byteCode += ".super java/lang/Object \n\n";

		// Fields
		/*
		* string typeTag (type Tag - i.e. Time)
		* string constrTag (constructor Tag - i.e. Hour or Min)
		* ADTByteCode[] (i.e. int or Age Addr)
		* int value
		* bool value
		* string value
		*/
		byteCode += ".field public typeTag Ljava/lang/String; \n";
		byteCode += ".field public constrTag Ljava/lang/String; \n";
		byteCode += ".field public objs [LADTByteCode; \n";
		byteCode += ".field public intVal I \n";
		byteCode += ".field public boolVal Z \n";
		byteCode += ".field public strVal Ljava/lang/String; \n";

		// Constructor
		byteCode += "\n.method public <init>()V \n";
		byteCode += ".limit stack 5 \n";
		byteCode += ".limit locals 10 \n";
		byteCode += "\t aload_0 \n";
		byteCode += "\t invokespecial java/lang/Object/<init>()V \n";
		byteCode += "\t return \n";
		byteCode += ".end method \n";

		return byteCode;
	}	
};