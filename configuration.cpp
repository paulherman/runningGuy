#include "configuration.hpp"

string ConfigurationMF::readString(char *tag, char *filename)
{
	unsigned int t_Count = 0;
	bool t_Found = false;
	while (t_Count < m_Variables.size() && t_Found == false)
	{
		if (strcmp(m_Variables[t_Count].m_Tag.c_str(), tag) == 0 && strcmp(m_Variables[t_Count].m_Filename.c_str(), filename) == 0)
			t_Found = true;
		else
			t_Count++;
	}
	return m_Variables[t_Count].m_Value;
}
int ConfigurationMF::readInt(char *tag, char *filename)
{
	string t_String = readString(tag, filename);
	return atoi(t_String.c_str());
}
bool ConfigurationMF::load(char *filename)
{
	m_File.open(filename);
	string t_Line;
	char t_ReadChar;
	int t_Count;
	int t_Size;
	bool t_End;
	bool t_Skip;
	bool t_Return = true;
	string::iterator t_Iterator;
	config t_Config;
	t_Config.m_Filename = filename;
	while (m_File.eof() == false)
	{
		t_Line.clear();
		t_Config.m_Tag.clear();
		t_Config.m_Value.clear();
		do {
			m_File.get(t_ReadChar);
			t_Line.push_back(t_ReadChar);
		} while (t_ReadChar != '\n' && m_File.eof() == false);
		t_Count = 0;
		t_Size = t_Line.length()-1;
		t_End = false;
		t_Skip = false;
		if (t_Line[t_Size] == '\n')
		{
			t_Iterator = t_Line.begin() + t_Size;
			t_Line.erase(t_Iterator);
		}
		if (t_Line[0] != '#' && t_Size != 0)
		{
			//Get the tag
			while (t_Line[t_Count] != ' ' && t_Count < t_Size)
			{
				t_Config.m_Tag.push_back(t_Line[t_Count]);
				t_Count++;
			}
			//Skip the next three characters ' = '
			t_Count = t_Count + 3;
			//Skip the beginning of the value '"'
			t_Count = t_Count + 1;
			if (t_Count >= t_Size)
			{
				t_End = true;
				t_Return = false;
			}
			//Get the value considering that '\' skips the meaning of the next character
			while (t_End == false)
			{
				if (t_Line[t_Count] == '"' && t_Skip == false)
				{
					t_End = true;
				}
				else if (t_Line[t_Count] == '\\' && t_Skip == false)
				{
					t_Skip = true;
				}
				else if (t_Skip == true)
				{
					switch(t_Line[t_Count])
					{
						case '"':
						{
							t_Config.m_Value.push_back('"');
							break;
						}
						case 'n':
						{
							t_Config.m_Value.push_back('\n');
							break;
						}
						case '\\':
						{
							t_Config.m_Value.push_back('\\');
							break;
						}
						default:
						{
							break;
						}
					}
					t_Skip = false;
				}
				else if (t_Skip == false)
					t_Config.m_Value.push_back(t_Line[t_Count]);
				t_Count++;
				if (t_Count >= t_Size)
				{
					t_Return = false;
					t_End = true;
				}
			}
			m_Variables.push_back(t_Config);
		}
	}
	if (m_File.is_open() == true)
		m_File.close();
	return t_Return;
}
string Configuration::readString(char *tag)
{
	unsigned int t_Count = 0;
	bool t_Found = false;
	while (t_Count < m_Variables.size() && t_Found == false)
	{
		if (strcmp(m_Variables[t_Count].m_Tag.c_str(), tag) == 0)
			t_Found = true;
		else
			t_Count++;
	}
	return m_Variables[t_Count].m_Value;
}
int Configuration::readInt(char *tag)
{
	string t_String = readString(tag);
	return atoi(t_String.c_str());
}
bool Configuration::load(char *filename)
{
	m_File.open(filename);
	string t_Line;
	char t_ReadChar;
	int t_Count;
	int t_Size;
	bool t_End;
	bool t_Skip;
	bool t_Return = true;
	string::iterator t_Iterator;
	config t_Config;
	while (m_File.eof() == false)
	{
		t_Line.clear();
		t_Config.m_Tag.clear();
		t_Config.m_Value.clear();
		do {
			m_File.get(t_ReadChar);
			t_Line.push_back(t_ReadChar);
		} while (t_ReadChar != '\n' && m_File.eof() == false);
		t_Count = 0;
		t_Size = t_Line.length()-1;
		t_End = false;
		t_Skip = false;
		if (t_Line[t_Size] == '\n')
		{
			t_Iterator = t_Line.begin() + t_Size;
			t_Line.erase(t_Iterator);
		}
		if (t_Line[0] != '#' && t_Size != 0)
		{
			//Get the tag
			while (t_Line[t_Count] != ' ' && t_Count < t_Size)
			{
				t_Config.m_Tag.push_back(t_Line[t_Count]);
				t_Count++;
			}
			//Skip the next three characters ' = '
			t_Count = t_Count + 3;
			//Skip the beginning of the value '"'
			t_Count = t_Count + 1;
			if (t_Count >= t_Size)
			{
				t_End = true;
				t_Return = false;
			}
			//Get the value considering that '\' skips the meaning of the next character
			while (t_End == false)
			{
				if (t_Line[t_Count] == '"' && t_Skip == false)
					t_End = true;
				else if (t_Line[t_Count] == '\\' && t_Skip == false)
					t_Skip = true;
				else if (t_Skip == true)
				{
					switch(t_Line[t_Count])
					{
						case '"':
						{
							t_Config.m_Value.push_back('"');
							break;
						}
						case 'n':
						{
							t_Config.m_Value.push_back('\n');
							break;
						}
						case '\\':
						{
							t_Config.m_Value.push_back('\\');
							break;
						}
						default:
						{
							break;
						}
					}
					t_Skip = false;
				}
				else if (t_Skip == false)
					t_Config.m_Value.push_back(t_Line[t_Count]);
				t_Count++;
				if (t_Count >= t_Size)
				{
					t_Return = false;
					t_End = true;
				}
			}
			m_Variables.push_back(t_Config);
		}
	}
	if (m_File.is_open() == true)
		m_File.close();
	return t_Return;
}
