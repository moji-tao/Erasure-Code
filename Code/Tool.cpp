#include "Tool.h"

std::string Hash_MD5::getHash(std::string data)
{
	Init();
	Update(data);

	unsigned char md[16];
	memset(md, 0x00, sizeof(md));
	MD5_Final(md, &ctx);

	char result[33];
	memset(result, 0x00, sizeof(result));
	for (int i = 0; i < 16; ++i)
	{
		sprintf(&result[i * 2], "%02x", md[i]);
	}
	return std::string(result);
}

void Hash_MD5::Init()
{
	MD5_Init(&ctx);
}

void Hash_MD5::Update(std::string data)
{
	MD5_Update(&ctx, data.c_str(), data.length());
}
