#include "ToIn.h"
#include "json/json.h"
#include <ostream>
using namespace std;

// DeBug����  Release������
int main()
{

	ToEnCode("E:\\one\\ECTest\\test", "C:\\Users\\mojitao\\Documents\\Tencent Files\\1029487273\\FileRecv\\��Ŀ��ʾ��Ƶ_Hello Cloud.avi", 6, 6);

	printf("�����룺");
	getchar();

	ToDeCode("E:\\one\\ECTest\\test", "E:\\one\\ECTest\\test", "��Ŀ��ʾ��Ƶ_Hello Cloud.avi");
	
	return 0;
}