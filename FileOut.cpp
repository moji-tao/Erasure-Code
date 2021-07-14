#include "FileOut.h"
#include <algorithm>

FileOut::FileOut():
	isClose(true)
{
}

FileOut::FileOut(std::string fileName):
	File(fileName)
{
	fileOut = std::ofstream(fileName, std::ios::out | std::ios::binary | std::ios::app);
	//fileOut = std::ofstream(fileName, std::ios::out);
	if (fileOut)
	{
		isClose = false;
	}
}

FileOut::~FileOut()
{
	if (!isClose)
	{
		isClose = true;
		fileOut.close();
	}

}

void FileOut::inputText(const void* ch, unsigned long long size)
{
	fileOut.write((const char*)ch, size);
}

void FileOut::inputInt(long long num)
{
	std::string str;
	str += char('0' + num % 10);
	num /= 10;
	while (num != 0)
	{
		str += char('0' + num % 10);
		num /= 10;
	}
	reverse(str.begin(), str.end());
	fileOut << str;
}

void FileOut::enter()
{
	fileOut << "\r\n";
}

void FileOut::CloseFile()
{
	if (!isClose)
	{
		fileOut.close();
		isClose = true;
	}
}

void FileOut::OpenFile(std::string fileName)
{
	if (!isClose)
	{
		//TODO:
		return;
	}

	fileOut.open(fileName, std::ios::out | std::ios::binary | std::ios::app);
	isClose = false;
	return;
}

FileOut& operator<<(FileOut& out, const std::string& str)
{
	out.fileOut << str;
	return out;
}
