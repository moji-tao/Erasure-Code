#pragma once
#include<fstream>

//��װ�ļ�IO��
class File
{
public:
	File(std::string str);
	File();

	const unsigned int MaxThreadNum = 6;

private:
	std::string fileName;
};