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
	std::cout << "��ȡ������Ϣ���" << endl;
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
		printf("�ļ�������ɣ���ʱ��%f\n", (double)(end - start) / CLK_TCK);

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
	//�Ѷ�ȡ�ļ�
	unsigned long long count = 0;

	//�����ļ���
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
			//�ļ�������
			continue;
		}

		//��ȡ�ļ�
		//std::cout << "���ڶ�ȡ��" << i << "���ļ�������" << endl;
		//std::cout << getPath(i) << endl;
		fileIn.read(ch, (sizeof(long) * MaxFileSize) / (stepByteSize * k), loopTime);
		fileIn.closeFile();
		//std::cout << " �ļ���ȡ���" << endl;

		//�����������
		fileRow.fill(0);
		for (int i = 0; i < fileRow.n_cols; ++i)
		{
			num = 0;
			memcpy(&num, ch + (i * sizeof(long)), sizeof(long));
			fileRow.at(0, i) = num;
		}
		//fileRow.print("�ļ��о���");
		//std::cout << "  �ļ��������ɹ������ںϲ�����" << endl;
		S = Matrix::verticalJoin(S, fileRow);
		//std::cout << "   ����ϲ��ɹ�" << endl;
		delete[] ch;

		//��һ��ѭ����¼���ļ���Ƭ��Ӧ������
		if (loopTime == 0)
		{
			arr.push_back(i - 1);
		}

		//�ļ���Ƭ�ﵽk�˳���ȡ����ʼ����
		++count;
		if (count >= k)
		{
			printf("�ļ���ȡ���\n");
			return true;
		}
	}

	//�����ļ�����
	std::cout << "�����ļ����㣡" << endl;
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
	//����ǵ�һ��ѭ�����õ���������Ӧ�����ɾ���
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
		printf("��������ȡ���\n");
	}

	/*
	printf("DeCode:\n");
	Matrix::print(deCode);
	*/
	/*
	printf("�����ļ�����\n");
	Matrix::print(S);
	*/
	clock_t start, end;
	start = clock();

	//��������
	S = inv(*deCode) * S;
	/*
	printf("�����ļ�����\n");
	Matrix::print(ed);
	*/
	//ed.print("�����ļ�����");
	end = clock();
	printf("����������ɣ���ʱ��%f\n", (double)(end - start) / CLK_TCK);

	//����ת�����ַ���
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

	printf("�ļ��������...\n");
	start = clock();
	//�ļ����
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
	printf("�ļ������ɣ���ʱ��%f\n", (double)(end - start) / CLK_TCK);
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
