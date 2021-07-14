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
	//ԭʼ�ļ���
	DeCode();
	~DeCode();
	bool OpenFile(std::string fragmentDir, std::string fileName);
	bool DeCoding(std::string path);

private:
	//���ر����ļ�����������ļ������򷵻�false
	bool LoadFile();
	//��ȡ·��
	std::string getPath(unsigned long long index);
	//��������
	void Coding(std::string path);
	//��ȡ������Ϣ
	bool GetInfo(std::ifstream& in);

	std::string getStringNum(int num);

private:
	mat S;    // �ļ�����
	mat* deCode;  //�������
	FileIn fileIn;
	FileOut fileOut;
	std::string fileName;  // �����ļ��� 
	std::string dir = "test\\";
	std::vector<unsigned int> arr;
	unsigned int loopTime;
	unsigned long long fileBlockCount;
	Coded saved;
};