#include "Matrix.h"

mat Matrix::horizontalJoin(const mat& A, const mat& B)
{
	return arma::join_horiz(A, B);
}

mat Matrix::verticalJoin(const mat& A, const mat& B)
{
	return arma::join_vert(A, B);
}

mat Matrix::createIdentity(unsigned long long m, unsigned long long n)
{
	return mat().eye(m, n);
}

mat Matrix::createIdentity(unsigned long long n)
{
	return mat().eye(n, n);
}

mat Matrix::createVandermonde(unsigned long long m, unsigned long long n)
{
	mat vandermonde(m, n);
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			vandermonde.at(i, j) = std::pow(i + 1, j);
		}
	}
	return vandermonde;
}

mat Matrix::createRandom(unsigned long long m, unsigned long long n, unsigned long long numRand)
{
	mat mm(m, n);
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			mm.at(i, j) = rand() % numRand;
			while (mm.at(i, j) == 0)
			{
				mm.at(i, j) = rand() % numRand;
			}
		}
	}
	return mm;
}

mat Matrix::createRandom(unsigned long long n, unsigned long long rand)
{
	return randu<mat>(n, n) * rand;
}

mat Matrix::inv(const mat& M)
{
	return M.i();
}

void Matrix::print(mat& mm)
{
	for (int i = 0; i < mm.n_rows; ++i)
	{
		for (int j = 0; j < mm.n_cols; ++j)
		{
			printf("%10.lf", mm.at(i, j));
		}
		printf("\n");
	}
}
