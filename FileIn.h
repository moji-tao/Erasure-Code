#pragma once
#include "File.h"
#include <string>
#include <sys/stat.h>

class FileIn : public File
{
public:
	//编码用
	FileIn(std::string fileName);
	//解码用
	FileIn();
	~FileIn();

	//打开文件
	bool openFile(std::string fileName);
	//关闭文件
	bool closeFile();
	//获取文件大小
	unsigned long long getFileSize();
	//读取文件（参数自动new，需要手动delete）
	int read(char* &ch, unsigned long long length, unsigned int loopTime);
	//检测文件是否打开
	bool isOpen();
	
	friend FileIn& operator>>(FileIn& in, std::string& str);

private:
	//文件是否关闭
	bool isClose;
	std::string fileName;
	std::ifstream fileIn;
	unsigned long long fileSize;
};