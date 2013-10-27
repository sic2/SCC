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
		byteCode += ".super java/lang/Object \n";
		byteCode += ".class public ADTByteCode \n\n";

		// Fields
		byteCode += ".field public tag Ljava/lang/String; \n";

		// Constructor
		byteCode += "\n.method public <init>()V \n";
		byteCode += ".limit stack 5 \n";
		byteCode += ".limit locals 100 \n";
		byteCode += "\t aload_0 \n";
		byteCode += "\t invokespecial java/lang/Object/<init>()V \n";
		byteCode += "\t return \n";
		byteCode += ".end method \n";

		return byteCode;
	}	
};