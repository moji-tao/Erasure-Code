#include "Code.h"

Code::Code(unsigned long long k1, unsigned long long m1):
	k(k1),m(m1),stepByteSize(MaxStepByteSize)
{
	
}

Code::Code():
	stepByteSize(MaxStepByteSize)
{
}

Code::~Code()
{
}

void Code::SetExtendByteSize(unsigned long long fileSize, bool isCut)
{
	if (isCut)
	{
		extendByteSize = (MaxFileSize - (fileSize % MaxFileSize));
		blockSize = MaxFileSize / k;
	}
	else
	{
		extendByteSize = (k * stepByteSize) - (fileSize % (k * stepByteSize));
		blockSize = (extendByteSize + fileSize) / (k);
	}
}

Coded::Coded(std::string path, unsigned long long fileSize, unsigned long long k, unsigned long long m,
	         unsigned long long blockSize, unsigned long long extendByteSize, mat G):
	path(path), fileSize(fileSize), k(k), m(m), blockSize(blockSize), extendByteSize(extendByteSize), G(G)
{
}

Coded::Coded()
{
}

void Coded::setFileInfo(std::string path, unsigned long long fileSize, unsigned long long MaxFileSize, unsigned long long k, unsigned long long m,
	unsigned long long blockSize, unsigned long long extendByteSize, bool isCut)
{
	this->path = path;
	this->fileSize = fileSize;
	this->MaxFileSize = MaxFileSize;
	this->k = k;
	this->m = m;
	this->blockSize = blockSize;
	this->extendByteSize = extendByteSize;
	this->isCut = isCut;
}

void Coded::setG(const mat& G)
{
	this->G = G;
}
