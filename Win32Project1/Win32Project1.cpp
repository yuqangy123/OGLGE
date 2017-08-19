// Win32Project1.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "shaders.h"
//#include "HelloTriangle/helloTriangle.h"
#include "OGLGE.h"
using namespace std;


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

