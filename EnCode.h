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

	//����
	bool EnCoding();
	//��ȡ�������
	mat getG();
	
private:
	//�����ļ�
	void LoadFile();
	//�������
	void Dot();
	//��������ļ�
	void OutPutFile();
	//��ȡ����ļ���
	std::string getOutFileName(unsigned int i);
	//��ȡ����ļ�����
	unsigned long long getCount();
	//�ļ����
	void extendText(void* ch);
	//���������Ϣ
	void SaveInfo();

private:
	FileIn fileIn;
	std::string path;
	std::string fileName;
	unsigned long long fileSize;
	unsigned int loopTime;
	Coded saved;
	bool isCut;
	
	//�������
	mat* G;
	//ԭ�ļ�����
	mat* sourceFile;
	//�����ļ�����
	mat* destFile;

	//�����ļ�����
	unsigned long long count;

	std::string dir = "test\\";
};