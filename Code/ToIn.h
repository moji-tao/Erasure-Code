#include <iostream>
#include <armadillo>//����armadilloͷ�ļ�
#include <cmath>
#include <unordered_map>
#include "EnCode.h"
#include "DeCode.h"
#include "Tool.h"
#include <ctime>
#include <thread>

//��Ƭ����·�� �����ļ��� ԭʼ��Ƭ�� ��չ��Ƭ��
extern "C" __declspec(dllexport) int ToEnCode(const char* fragmentDir, const char* fileName, int k, int m);
//��Ƭ����·�� �ļ����·�� �ļ���
extern "C" __declspec(dllexport) int ToDeCode(const char* fragmentDir, const char* fileInputDir, const char* fileName);