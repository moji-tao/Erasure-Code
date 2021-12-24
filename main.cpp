#include "ToIn.h"
#include "json/json.h"
#include <ostream>
using namespace std;

// DeBug可用  Release不可用
int main()
{

	ToEnCode("E:\\one\\ECTest\\test", "C:\\Users\\mojitao\\Documents\\Tencent Files\\项目演示视频_Hello Cloud.avi", 6, 6);

	printf("请输入：");
	getchar();

	ToDeCode("E:\\one\\ECTest\\test", "E:\\one\\ECTest\\test", "项目演示视频_Hello Cloud.avi");
	
	return 0;
}
