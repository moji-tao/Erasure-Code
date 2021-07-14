#include "EnCode.h"
#include <armadillo>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <json/json.h>

EnCode::EnCode(int k1, int m1, std::string dir, std::string file) :
	Code(k1, m1), path(file), fileIn(file), count(0), 
	G(nullptr), sourceFile(nullptr), destFile(nullptr), 
	fileSize(0), isCut(false), loopTime(0)
{
	this->dir = dir + "\\";
	if (fileIn.isOpen())
	{
		this->fileSize = fileIn.getFileSize();
		if (fileSize > MaxFileSize)
		{
			isCut = true;
			MaxFileSize = MaxFileSize - (MaxFileSize % (3 * k));
			SetExtendByteSize(fileSize, isCut);
		}
		else
		{
			SetExtendByteSize(fileSize, isCut);
			MaxFileSize = fileSize + extendByteSize;
		}
	}
	else
		printf("δ�ҵ��ļ�\n");
	
}

EnCode::~EnCode()
{
	if (G != nullptr)
		delete G;
	if (sourceFile != nullptr)
		delete sourceFile;
	if (destFile != nullptr)
		delete destFile;
}

bool EnCode::EnCoding()
{
	if (!fileIn.isOpen())
	{
		return false;
	}
	size_t index = path.find_last_of('\\');
	if (index == -1)
	{
		fileName = path;
	}
	else
	{
		fileName = std::string(path.begin() + index + 1, path.end());
	}

	//�����ļ����� ���ɱ������
	saved.setFileInfo(path, fileIn.getFileSize(), MaxFileSize, k, m, blockSize, extendByteSize, isCut);
	
	saved.G = Matrix::createRandom(m + k, k, 128 / k);
	//Saved[fileName].G = Matrix::createIdentity(m + k, k);

	G = &saved.G;
	//G->print("EnCode��");
	
	//���پ���ռ�
	sourceFile = new mat(k, blockSize / stepByteSize);
	destFile = new mat(saved.G.n_rows, blockSize / stepByteSize);

	//����ָ��ļ�����ѭ���������ָ��ļ�ֻ��һ��
	do
	{
		LoadFile();
		Dot();
		OutPutFile();
		++loopTime;

	} while (isCut);

	//��������
	delete sourceFile;
	sourceFile = nullptr;
	delete destFile;
	destFile = nullptr;

	saved.loopTime = loopTime;
	G = nullptr;
	SaveInfo();
	return true;
}

void EnCode::LoadFile()
{
	clock_t start, end;
	start = clock();
	//��ȡ�ļ������
	char* text = nullptr;
	unsigned long long length = fileIn.read(text, MaxFileSize, loopTime);
	if (length == 0)
	{
		extendText(text);
		isCut = false;
	}
	end = clock();
	printf("�ļ�����ɹ�����ʱ��%f\n", (double)(end - start) / CLK_TCK);

	start = clock();
	//�����ļ����ݾ���
	unsigned long count = 0;
	long num = 0;

	for (int i = 0; i < sourceFile->n_rows; ++i)
	{
		for (int j = 0; j < sourceFile->n_cols; ++j)
		{
			//һ�ζ�ȡlong���ֽڣ���λ����
			memcpy(&num, text + (count * stepByteSize), stepByteSize + 1);
			memset(((char*)(&num) + 3), 0x00, 1);
			sourceFile->at(i, j) = num;
			++count;
		}
	}
	end = clock();
	printf("�ļ������������ʱ��%f\n", (double)(end - start) / CLK_TCK);

	delete[] text;
	/*
	printf("ԭ�ļ�����\n");
	Matrix::print(*sourceFile);
	*/
}

void EnCode::Dot()
{	
	clock_t start, end;
	start = clock();
	//��������
	*destFile = (saved.G) * (*sourceFile);
	/*
	printf("������ļ�����\n");
	Matrix::print(*destFile);
	*/
	//����ԭ�ļ�����

	end = clock();
	printf("����������ɣ���ʱ��%f\n", (double)(end - start) / CLK_TCK);
}

void EnCode::OutPutFile()
{
	clock_t start, end;
	start = clock();

	FileOut fileOut;
	char* ch = nullptr;
	unsigned long long length = destFile->n_cols * sizeof(long);
	long num;

	ch = new char[length];
	//��ÿ�������ļ�д������
	for (int i = 0; i < destFile->n_rows; ++i)
	{
		memset(ch, 0x00, length);
		fileOut.OpenFile(getOutFileName(i + 1));
		for (int j = 0; j < destFile->n_cols; ++j)
		{
			num = destFile->at(i, j);
			memcpy(ch + (j * sizeof(long)), (char*)&num, sizeof(long));
		}
		fileOut.inputText(ch, length);
		fileOut.CloseFile();
		
	}
	delete[] ch;

	end = clock();
	printf("�ļ�д����ɣ���ʱ��%f\n", (double)(end - start) / CLK_TCK);
}

std::string EnCode::getOutFileName(unsigned int i)
{
	//TODU:�����Ż�
	std::string num;
	while (i != 0)
	{
		num += char('0' + i % 10);
		i /= 10;
	}
	reverse(num.begin(), num.end());
	num = md5.getHash(fileName + num);

	return std::string(dir + num);
}

unsigned long long EnCode::getCount()
{
	return m + k;
}

void EnCode::extendText(void* ch)
{
	char* ext = nullptr;
	if (isCut)
	{
		ext = (char*)ch + (fileSize % MaxFileSize);
	}
	else
	{
		ext = (char*)ch + fileSize;
	}

	int num;
	for (int i = 0; i < extendByteSize; ++i)
	{
		num = rand() % 127;
		memcpy(ext + i, &num, 1);
	}
}

void EnCode::SaveInfo()
{
	//std::string path;    // ԭ�ļ�·��
	//unsigned long long fileSize;  //ԭ�ļ���С
	//unsigned long long MaxFileSize;  //  ������Ƭ�ֽ��������ǵ����ļ���
	//unsigned long long k;    // ��Ƭ��(������Ҫ���ļ���)
	//unsigned long long m;    // ����
	//unsigned long long blockSize; // ����Ƭ��С
	//unsigned long long extendByteSize;    // ��չ�ֽ���
	//bool isCut; //�Ƿ��Ƭ
	//unsigned int loopTime;
	//mat G;
	Json::Value root;
	root["path"] = Json::Value(saved.path);
	root["fileSize"] = Json::Value(unsigned int(saved.fileSize));
	root["MaxFileSize"] = Json::Value(unsigned int(saved.MaxFileSize));
	root["k"] = Json::Value(unsigned int(saved.k));
	root["m"] = Json::Value(unsigned int(saved.m));
	root["blockSize"] = Json::Value(unsigned int(saved.blockSize));
	root["extendByteSize"] = Json::Value(unsigned int(saved.extendByteSize));
	root["isCut"] = Json::Value(saved.isCut);
	root["loopTime"] = Json::Value(unsigned int(saved.loopTime));
	
	for (int i = 0; i < saved.G.n_rows; ++i)
	{
		for (int j = 0; j < saved.G.n_cols; ++j)
		{
			root["G"].append(saved.G.at(i, j));
		}
	}
	Json::StyledWriter sw;
	FileOut info;
	info.OpenFile(dir + fileName + "Info.json");
	info << sw.write(root);
}

mat EnCode::getG()
{
	return *G;
}
