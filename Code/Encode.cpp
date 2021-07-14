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
		printf("未找到文件\n");
	
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

	//保存文件数据 生成编码矩阵
	saved.setFileInfo(path, fileIn.getFileSize(), MaxFileSize, k, m, blockSize, extendByteSize, isCut);
	
	saved.G = Matrix::createRandom(m + k, k, 128 / k);
	//Saved[fileName].G = Matrix::createIdentity(m + k, k);

	G = &saved.G;
	//G->print("EnCode：");
	
	//开辟矩阵空间
	sourceFile = new mat(k, blockSize / stepByteSize);
	destFile = new mat(saved.G.n_rows, blockSize / stepByteSize);

	//如果分割文件，则循环读；不分割文件只读一次
	do
	{
		LoadFile();
		Dot();
		OutPutFile();
		++loopTime;

	} while (isCut);

	//矩阵析构
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
	//读取文件并填充
	char* text = nullptr;
	unsigned long long length = fileIn.read(text, MaxFileSize, loopTime);
	if (length == 0)
	{
		extendText(text);
		isCut = false;
	}
	end = clock();
	printf("文件载入成功，用时：%f\n", (double)(end - start) / CLK_TCK);

	start = clock();
	//生成文件数据矩阵
	unsigned long count = 0;
	long num = 0;

	for (int i = 0; i < sourceFile->n_rows; ++i)
	{
		for (int j = 0; j < sourceFile->n_cols; ++j)
		{
			//一次读取long个字节，高位置零
			memcpy(&num, text + (count * stepByteSize), stepByteSize + 1);
			memset(((char*)(&num) + 3), 0x00, 1);
			sourceFile->at(i, j) = num;
			++count;
		}
	}
	end = clock();
	printf("文件已填入矩阵，用时：%f\n", (double)(end - start) / CLK_TCK);

	delete[] text;
	/*
	printf("原文件矩阵：\n");
	Matrix::print(*sourceFile);
	*/
}

void EnCode::Dot()
{	
	clock_t start, end;
	start = clock();
	//矩阵运算
	*destFile = (saved.G) * (*sourceFile);
	/*
	printf("编码后文件矩阵\n");
	Matrix::print(*destFile);
	*/
	//销毁原文件矩阵

	end = clock();
	printf("矩阵运算完成，用时：%f\n", (double)(end - start) / CLK_TCK);
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
	//向每个编码文件写入数据
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
	printf("文件写入完成，用时：%f\n", (double)(end - start) / CLK_TCK);
}

std::string EnCode::getOutFileName(unsigned int i)
{
	//TODU:参数优化
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
	//std::string path;    // 原文件路径
	//unsigned long long fileSize;  //原文件大小
	//unsigned long long MaxFileSize;  //  单个分片字节数（不是单个文件）
	//unsigned long long k;    // 分片数(最少需要的文件数)
	//unsigned long long m;    // 增广
	//unsigned long long blockSize; // 单分片大小
	//unsigned long long extendByteSize;    // 扩展字节数
	//bool isCut; //是否分片
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
