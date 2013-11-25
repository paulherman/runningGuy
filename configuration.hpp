#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;

class ConfigurationMF
{
public:
	bool load(char *filename);
	int readInt(char *tag, char *filename);
	string readString(char *tag, char *filename);
private:
	struct config
	{
		string m_Tag;
		string m_Value;
		string m_Filename;
	};
	ifstream m_File;
	vector<config> m_Variables;
};
class Configuration
{
public:
	bool load(char *filename);
	int readInt(char *tag);
	string readString(char *tag);
private:
	struct config
	{
		string m_Tag;
		string m_Value;
	};
	ifstream m_File;
	vector<config> m_Variables;
};
