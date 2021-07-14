#pragma once
#include<fstream>

//封装文件IO流
class File
{
public:
	File(std::string str);
	File();

	const unsigned int MaxThreadNum = 6;

private:
	std::string fileName;
};