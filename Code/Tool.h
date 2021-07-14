#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

class Hash_MD5
{
public:
	std::string getHash(std::string data);

private:
	void Init();
	void Update(std::string data);

private:
	MD5_CTX ctx;
};

