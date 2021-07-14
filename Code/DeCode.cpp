#include "DeCode.h"
#include <ctime>
#include <json/json.h>
#include <istream>

DeCode::DeCode()
	:deCode(nullptr),loopTime(0)
{
}

DeCode::~DeCode()
{
	if (deCode != nullptr)
		delete deCode;
}

bool DeCode::GetInfo(std::ifstream& in)
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
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(in, root))
	{
		saved.path = std::string(root["path"].asCString());
		saved.fileSize = root["fileSize"].asUInt();
		MaxFileSize = saved.MaxFileSize = root["MaxFileSize"].asUInt();
		k = saved.k = root["k"].asUInt();
		m = saved.m = root["m"].asUInt();
		blockSize = saved.blockSize = root["blockSize"].asUInt();
		extendByteSize = saved.extendByteSize = root["extendByteSize"].asUInt();
		saved.isCut = root["isCut"].asBool();
		saved.loopTime = root["loopTime"].asUInt();
		saved.G = mat(saved.m + saved.k, saved.k);
		fileBlockCount = saved.k + saved.m;
		unsigned int count = 0;
		for (int i = 0; i < saved.G.n_rows; ++i)
		{
			for (int j = 0; j < saved.G.n_cols; ++j)
			{
				saved.G.at(i, j) = root["G"][count++].asUInt();
			}
		}
	}
	else
		return false;
	return true;
}

bool DeCode::OpenFile(std::string fragmentDir, std::string fileName)
{
	std::string path(fragmentDir + "\\" + fileName + "Info.json");
	std::ifstream in(path, ios::binary);
	if (!in.is_open())
	{
		return false;
	}
	this->dir = fragmentDir + "\\";
	this->fileName = fileName;
	arr.clear();
	S.clear();
	GetInfo(in);
	in.close();
	std::cout << "获取编码信息完成" << endl;
}

bool DeCode::DeCoding(std::string path)
{
	if (fileName == "")
	{
		return false;
	}

	clock_t start, end;
	
	do {
		start = clock();
		if (!LoadFile())
		{
			return false;
		}
		end = clock();
		printf("文件加载完成，用时：%f\n", (double)(end - start) / CLK_TCK);

		Coding(path);

		S.reset();
		++loopTime;
	} while (loopTime < saved.loopTime);

	if (deCode != nullptr)
	{
		delete deCode;
		deCode = nullptr;
	}

	return true;
}

bool DeCode::LoadFile()
{
	//已读取文件
	unsigned long long count = 0;

	//解码文件行
	mat fileRow(1, blockSize / stepByteSize);
	
	char* ch = nullptr;
	long long num = 0;
	for (int i = 1; i <= fileBlockCount; ++i)
	{
		if (count >= k)
		{
			break;
		}
		if (!fileIn.openFile(getPath(i)))
		{
			//文件不存在
			continue;
		}

		//读取文件
		//std::cout << "正在读取第" << i << "个文件到矩阵" << endl;
		//std::cout << getPath(i) << endl;
		fileIn.read(ch, (sizeof(long) * MaxFileSize) / (stepByteSize * k), loopTime);
		fileIn.closeFile();
		//std::cout << " 文件读取完成" << endl;

		//数据填入矩阵
		fileRow.fill(0);
		for (int i = 0; i < fileRow.n_cols; ++i)
		{
			num = 0;
			memcpy(&num, ch + (i * sizeof(long)), sizeof(long));
			fileRow.at(0, i) = num;
		}
		//fileRow.print("文件行矩阵：");
		//std::cout << "  文件导入矩阵成功，正在合并矩阵" << endl;
		S = Matrix::verticalJoin(S, fileRow);
		//std::cout << "   矩阵合并成功" << endl;
		delete[] ch;

		//第一次循环记录改文件碎片对应解码行
		if (loopTime == 0)
		{
			arr.push_back(i - 1);
		}

		//文件碎片达到k退出读取，开始解码
		++count;
		if (count >= k)
		{
			printf("文件读取完成\n");
			return true;
		}
	}

	//编码文件不足
	std::cout << "编码文件不足！" << endl;
	return false;
}

std::string DeCode::getPath(unsigned long long index)
{
	unsigned long long i = index;
	std::string num;
	while (i != 0)
	{
		num += char('0' + i % 10);
		i /= 10;
	}
	reverse(num.begin(), num.end());
	std::string file = md5.getHash(fileName + num);
	return std::string(dir + file);
}

void DeCode::Coding(std::string path)
{
	//如果是第一次循环，得到解码矩阵对应的生成矩阵
	if(loopTime == 0)
	{
		//mat deCode;
		deCode = new mat(k, saved.G.n_cols);
		for (int i = 0; i < k; ++i)
		{
			for (int j = 0; j < deCode->n_cols; ++j)
			{
				deCode->at(i, j) = saved.G.at(arr[i], j);
			}
		}
		printf("解码矩阵获取完成\n");
	}

	/*
	printf("DeCode:\n");
	Matrix::print(deCode);
	*/
	/*
	printf("解码文件矩阵：\n");
	Matrix::print(S);
	*/
	clock_t start, end;
	start = clock();

	//矩阵运算
	S = inv(*deCode) * S;
	/*
	printf("译码文件矩阵\n");
	Matrix::print(ed);
	*/
	//ed.print("译码文件矩阵");
	end = clock();
	printf("矩阵运算完成，用时：%f\n", (double)(end - start) / CLK_TCK);

	//矩阵转换成字符串
	char* text = new char[saved.MaxFileSize + saved.extendByteSize];
	memset(text, 0x00, saved.MaxFileSize + saved.extendByteSize);
	double num;
	long lNum;
	unsigned long long count = 0;
	for (int i = 0; i < S.n_rows; ++i)
	{
		for (int j = 0; j < S.n_cols; ++j)
		{
			num = S.at(i, j);
			lNum = (long)((num + 1e-5) > num ? (num + 1e-5) : num);
			memcpy(text + (count * stepByteSize), &lNum, stepByteSize);
			++count;
		}
	}

	printf("文件正在输出...\n");
	start = clock();
	//文件输出
	fileOut.OpenFile(path + fileName);
	if (saved.isCut)
	{
		if (loopTime + 1 != saved.loopTime)
		{
			fileOut.inputText(text, saved.MaxFileSize);
		}
		else
		{
			fileOut.inputText(text, saved.MaxFileSize - saved.extendByteSize);
		}
	}
	else
	{
		fileOut.inputText(text, saved.fileSize);
	}
	fileOut.CloseFile();
	end = clock();
	printf("文件输出完成，用时：%f\n", (double)(end - start) / CLK_TCK);
	delete[] text;
}

std::string DeCode::getStringNum(int num)
{
	std::string text;
	while (num != 0)
	{
		text += char('0' + num % 10);
		num /= 10;
	}
	reverse(text.begin(), text.end());
	return text;
}
