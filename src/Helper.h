#include <string>

/*
 * Use singleton pattern as described in 
 * Professional C++ - second edition. Gregorie, Solter and Kleper
 */
class Helper
{
	
public:
	static Helper& instance();
	
	void saveToFile(std::string fileName, std::string data);
	std::string integerToString(int value);
	
protected:
	static Helper sInstance;
	
private:
	Helper() {}
	virtual ~Helper() {}
};