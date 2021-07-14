#pragma once
#include <armadillo>
using namespace arma;

class Matrix
{
public:
	//两个矩阵水平拼接
	static mat horizontalJoin(const mat& A, const mat& B);

	//两个矩阵垂直拼接
	static mat verticalJoin(const mat& A, const mat& B);

	//生成大小为m*n的单位矩阵
	static mat createIdentity(unsigned long long m, unsigned long long n);
	static mat createIdentity(unsigned long long n);

	//生成大小为m*n的范德蒙矩阵
	static mat createVandermonde(unsigned long long m, unsigned long long n);

	//生成随机矩阵
	static mat createRandom(unsigned long long m, unsigned long long n, unsigned long long numRand);
	static mat createRandom(unsigned long long n, unsigned long long rand);

	static mat inv(const mat& M);

	//矩阵输出
	static void print(mat& mm);

private:
	
};