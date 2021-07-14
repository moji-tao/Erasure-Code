#pragma once
#include <armadillo>
using namespace arma;

class Matrix
{
public:
	//��������ˮƽƴ��
	static mat horizontalJoin(const mat& A, const mat& B);

	//��������ֱƴ��
	static mat verticalJoin(const mat& A, const mat& B);

	//���ɴ�СΪm*n�ĵ�λ����
	static mat createIdentity(unsigned long long m, unsigned long long n);
	static mat createIdentity(unsigned long long n);

	//���ɴ�СΪm*n�ķ����ɾ���
	static mat createVandermonde(unsigned long long m, unsigned long long n);

	//�����������
	static mat createRandom(unsigned long long m, unsigned long long n, unsigned long long numRand);
	static mat createRandom(unsigned long long n, unsigned long long rand);

	static mat inv(const mat& M);

	//�������
	static void print(mat& mm);

private:
	
};