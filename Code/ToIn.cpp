#include "ToIn.h"
using namespace std;

//unordered_map<std::string, Coded> Saved;

int ToEnCode(const char* fragmentDir, const char* fileName, int k, int m)
{
	EnCode* en = new EnCode(k, m, fragmentDir, fileName);
	en->EnCoding();
	delete en;
	return 0;
}

int ToDeCode(const char* fragmentDir, const char* fileInputDir, const char* fileName)
{
	string str(fileInputDir);
	DeCode* de = new DeCode();
	de->OpenFile(std::string(fragmentDir), fileName);
	de->DeCoding(str + "\\");
	delete de;
	return 0;
}
