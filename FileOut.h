#pragma once
#include "File.h"

class FileOut :public File
{
public:
	FileOut();
	FileOut(std::string fileName);
	~FileOut();

	//输入文本
	void inputText(const void* ch,unsigned long long size);

	//关闭文件
	void CloseFile();

	//打开文件
	void OpenFile(std::string fileName);

	//输入整形
	void inputInt(long long num);

	void enter();

	friend FileOut& operator<<(FileOut& out, const std::string& str);

private:
	std::ofstream fileOut;
	bool isClose;
};