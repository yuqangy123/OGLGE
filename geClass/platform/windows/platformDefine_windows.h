#pragma once

#define CC_STATIC

#if defined(CC_STATIC)
	#define OGLGE_DLL
#else
	#if defined(_USRDLL)
		#define OGLGE_DLL     __declspec(dllexport)
	#else         /* use a DLL library */
		#define OGLGE_DLL     __declspec(dllimport)
	#endif
#endif

