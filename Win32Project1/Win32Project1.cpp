// Win32Project1.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "shaders.h"
//#include "HelloTriangle/helloTriangle.h"
#include "OGLGE.h"
using namespace std;

//extern "C" { FILE __iob_func[3] = { *stdin, *stdout, *stderr }; }
#if _MSC_VER>=1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif 
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */
int main(int argc, char** argv)
{
	OGLGE::Instance()->init(argc, argv);
	OGLGE::Instance()->test();
	OGLGE::Instance()->start();





	return 0;

	int nSelect = 0;
	printf("enter your select:");
	scanf_s("%d", &nSelect);
	printf("\r\nselect %d\r\n", nSelect);
	switch (nSelect)
	{
		case 0:
		{
			
		};
		case 1:
		{
			/*
			auto gl = new helloTriangle();
			auto res = gl->init();
			if (0 != res)
				printf("init helloTriangle failed");
			*/
		}break;
	}
    return 0;
}

