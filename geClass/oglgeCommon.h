#ifndef __OGLGE_COMMON_H__
#define __OGLGE_COMMON_H__

	#include "platform/platformConfig.h"

	#if OGLGE_TARGET_PLATFORM && (OGLGE_TARGET_PLATFORM == OGLGE_PLATFORM_WIN32)
		#include "platform/windows/platformDefine_windows.h"

	#elif OGLGE_TARGET_PLATFORM && (OGLGE_TARGET_PLATFORM == OGLGE_PLATFORM_ANDROID)
		#include <platform/android/platformDefine_android.h>
	#endif

#endif