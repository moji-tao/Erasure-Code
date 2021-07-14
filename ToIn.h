#include <iostream>
#include <armadillo>//包含armadillo头文件
#include <cmath>
#include <unordered_map>
#include "EnCode.h"
#include "DeCode.h"
#include "Tool.h"
#include <ctime>
#include <thread>

//碎片保存路径 编码文件名 原始分片数 扩展分片数
extern "C" __declspec(dllexport) int ToEnCode(const char* fragmentDir, const char* fileName, int k, int m);
//碎片保存路径 文件输出路径 文件名
extern "C" __declspec(dllexport) int ToDeCode(const char* fragmentDir, const char* fileInputDir, const char* fileName);