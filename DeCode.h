#pragma once
#include "Code.h"
#include "FileIn.h"
#include "FileOut.h"
#include "Matrix.h"
#include <vector>

const unsigned long long MAXFILE = 64;

class DeCode : public Code
{
public:
	//原始文件名
	DeCode();
	~DeCode();
	bool OpenFile(std::string fragmentDir, std::string fileName);
	bool DeCoding(std::string path);

private:
	//加载编码文件，如果编码文件不足则返回false
	bool LoadFile();
	//获取路径
	std::string getPath(unsigned long long index);
	//解码运算
	void Coding(std::string path);
	//获取编码信息
	bool GetInfo(std::ifstream& in);

	std::string getStringNum(int num);

private:
	mat S;    // 文件矩阵
	mat* deCode;  //解码矩阵
	FileIn fileIn;
	FileOut fileOut;
	std::string fileName;  // 解码文件名 
	std::string dir = "test\\";
	std::vector<unsigned int> arr;
	unsigned int loopTime;
	unsigned long long fileBlockCount;
	Coded saved;
};