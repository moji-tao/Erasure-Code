#pragma once
#include "Tool.h"
#include <armadillo>
#include <unordered_map>
using namespace arma;

class Code
{
public:
	const unsigned long long MaxStepByteSize = 3;
	unsigned long long MaxFileSize = (200 * (1 << 20));

public:
	Code(unsigned long long k1, unsigned long long m1);
	Code();
	~Code();

protected:
	//设置扩展字节长度
	void SetExtendByteSize(unsigned long long fileSize, bool isCut);

protected:
	unsigned long long k;                 // 分片数
	unsigned long long m;                 // 增广
	unsigned long long blockSize;         // 单分片字节数
	unsigned long long extendByteSize;    // 扩展字节数
	unsigned long long stepByteSize;      // 一次读取stepByteSize个字节
	Hash_MD5 md5;
};

//文件编码信息
struct Coded
{
	std::string path;    // 原文件路径
	unsigned long long fileSize;  //原文件大小
	unsigned long long MaxFileSize;  //  单个分片字节数（不是单个文件）
	unsigned long long k;    // 分片数(最少需要的文件数)
	unsigned long long m;    // 增广
	unsigned long long blockSize; // 单分片大小
	unsigned long long extendByteSize;    // 扩展字节数
	bool isCut; //是否分片
	unsigned int loopTime;
	mat G;
	Coded(std::string path, unsigned long long fileSize, unsigned long long k, unsigned long long m,
		  unsigned long long blockSize, unsigned long long extendByteSize, mat G);
	Coded();
	void setFileInfo(std::string path, unsigned long long fileSize, unsigned long long MaxFileSize, unsigned long long k, unsigned long long m,
		unsigned long long blockSize, unsigned long long extendByteSize, bool isCut);
	void setG(const mat& G);
};

//extern std::unordered_map<std::string, Coded> Saved;