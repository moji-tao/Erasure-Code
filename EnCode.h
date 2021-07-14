#pragma once
#include "Code.h"
#include "FileIn.h"
#include "FileOut.h"
#include "Matrix.h"

class EnCode :public Code
{
public:
	EnCode(int k1, int m1,std::string dir,std::string file);
	~EnCode();

	//编码
	bool EnCoding();
	//获取编码矩阵
	mat getG();
	
private:
	//加载文件
	void LoadFile();
	//矩阵相乘
	void Dot();
	//输出编码文件
	void OutPutFile();
	//获取输出文件名
	std::string getOutFileName(unsigned int i);
	//获取输出文件个数
	unsigned long long getCount();
	//文件填充
	void extendText(void* ch);
	//保存编码信息
	void SaveInfo();

private:
	FileIn fileIn;
	std::string path;
	std::string fileName;
	unsigned long long fileSize;
	unsigned int loopTime;
	Coded saved;
	bool isCut;
	
	//编码矩阵
	mat* G;
	//原文件矩阵
	mat* sourceFile;
	//编码文件矩阵
	mat* destFile;

	//编码文件计数
	unsigned long long count;

	std::string dir = "test\\";
};