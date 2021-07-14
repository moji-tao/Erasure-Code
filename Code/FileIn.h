#pragma once
#include "File.h"
#include <string>
#include <sys/stat.h>

class FileIn : public File
{
public:
	//������
	FileIn(std::string fileName);
	//������
	FileIn();
	~FileIn();

	//���ļ�
	bool openFile(std::string fileName);
	//�ر��ļ�
	bool closeFile();
	//��ȡ�ļ���С
	unsigned long long getFileSize();
	//��ȡ�ļ��������Զ�new����Ҫ�ֶ�delete��
	int read(char* &ch, unsigned long long length, unsigned int loopTime);
	//����ļ��Ƿ��
	bool isOpen();
	
	friend FileIn& operator>>(FileIn& in, std::string& str);

private:
	//�ļ��Ƿ�ر�
	bool isClose;
	std::string fileName;
	std::ifstream fileIn;
	unsigned long long fileSize;
};