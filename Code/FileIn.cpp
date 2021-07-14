#include "FileIn.h"
#include <string>
#include <iostream>

FileIn::FileIn()
{
	isClose = true;
}

FileIn::FileIn(std::string fileName):
	File(fileName)
{
	fileIn = std::ifstream(fileName, std::ios::in | std::ios::binary);
	//fileIn = std::ifstream(fileName, std::ios::in);
	if (fileIn)
	{
		isClose = false;
		this->fileName = fileName;
	}
	else
	{
		isClose = true;
	}
	
}

FileIn::~FileIn()
{
	if (!isClose)
	{
		fileIn.close();
	}

}

bool FileIn::openFile(std::string fileName)
{
	if (!isClose)
	{
		return false;
	}

	fileIn.open(fileName, std::ios::in | std::ios::binary);
	//fileIn.open(fileName, std::ios::in);
	if (fileIn)
	{
		isClose = false;
		this->fileName = fileName;
		return true;
	}
	else
	{
		return false;
	}

}

bool FileIn::closeFile()
{
	if (!isClose)
	{
		fileIn.close();
		isClose = true;
	}
	return false;
}

unsigned long long FileIn::getFileSize()
{
	if (isClose)
	{
		return 0;
	}
	struct __stat64 st;
	__stat64(fileName.c_str(), &st);
	return st.st_size;
}

int FileIn::read(char*& ch, unsigned long long length, unsigned int loopTime)
{
	if (isClose)
	{
		std::cout << "文件未打开" << std::endl;
		return -1;
	}

	//开辟数组
	ch = new char[length];
	memset(ch, 0x00, length);

	//设置文件指针
	//fileIn >> std::noskipws;
	fileIn.seekg(loopTime * length, std::ios::beg);

	//读取文件
	fileIn.read(ch, length);

	fileSize = length;

	if (fileIn.peek() == EOF)
	{
		return 0;
	}

	return length;
}

bool FileIn::isOpen()
{
	if (fileIn)
		return true;
	else
		return false;
}

FileIn& operator>>(FileIn& in, std::string& str)
{
	in.fileIn >> str;
	return in;
}
