#pragma once
#include "File.h"

class FileOut :public File
{
public:
	FileOut();
	FileOut(std::string fileName);
	~FileOut();

	//�����ı�
	void inputText(const void* ch,unsigned long long size);

	//�ر��ļ�
	void CloseFile();

	//���ļ�
	void OpenFile(std::string fileName);

	//��������
	void inputInt(long long num);

	void enter();

	friend FileOut& operator<<(FileOut& out, const std::string& str);

private:
	std::ofstream fileOut;
	bool isClose;
};