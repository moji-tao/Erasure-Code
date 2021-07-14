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
	//������չ�ֽڳ���
	void SetExtendByteSize(unsigned long long fileSize, bool isCut);

protected:
	unsigned long long k;                 // ��Ƭ��
	unsigned long long m;                 // ����
	unsigned long long blockSize;         // ����Ƭ�ֽ���
	unsigned long long extendByteSize;    // ��չ�ֽ���
	unsigned long long stepByteSize;      // һ�ζ�ȡstepByteSize���ֽ�
	Hash_MD5 md5;
};

//�ļ�������Ϣ
struct Coded
{
	std::string path;    // ԭ�ļ�·��
	unsigned long long fileSize;  //ԭ�ļ���С
	unsigned long long MaxFileSize;  //  ������Ƭ�ֽ��������ǵ����ļ���
	unsigned long long k;    // ��Ƭ��(������Ҫ���ļ���)
	unsigned long long m;    // ����
	unsigned long long blockSize; // ����Ƭ��С
	unsigned long long extendByteSize;    // ��չ�ֽ���
	bool isCut; //�Ƿ��Ƭ
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