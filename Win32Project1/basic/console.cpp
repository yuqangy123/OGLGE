
#include "console.h"
#include "basic/base64.h"
#include <thread>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <io.h>
#include <WS2tcpip.h>
#include <Winsock2.h>
#if defined(__MINGW32__)
#include "platform/win32/inet_pton_mingw.h"
#endif
#define bzero(a, b) memset(a, 0, b);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#endif
#else
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#endif


#include "basic/base64.h"


extern const char* cocos2dVersion(void);

#define PROMPT  "> "

static const size_t SEND_BUFSIZ = 512;

void _log(const char *format, va_list args)
{
	int bufferSize = MAX_LOG_LENGTH;
	char* buf = nullptr;

	do
	{
		buf = new (std::nothrow) char[bufferSize];
		if (buf == nullptr)
			return; // not enough memory

		int ret = vsnprintf(buf, bufferSize - 3, format, args);
		if (ret < 0)
		{
			bufferSize *= 2;

			delete[] buf;
		}
		else
			break;

	} while (true);

	strcat(buf, "\n");

	#if OGLGE_TARGET_PLATFORM == OGLGE_PLATFORM_ANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info", "%s", buf);

	#elif OGLGE_TARGET_PLATFORM ==  OGLGE_PLATFORM_WIN32 || OGLGE_TARGET_PLATFORM == OGLGE_PLATFORM_WINRT

	int pos = 0;
	int len = strlen(buf);
	char tempBuf[MAX_LOG_LENGTH + 1] = { 0 };
	WCHAR wszBuf[MAX_LOG_LENGTH + 1] = { 0 };

	do
	{
		std::copy(buf + pos, buf + pos + MAX_LOG_LENGTH, tempBuf);

		tempBuf[MAX_LOG_LENGTH] = 0;

		MultiByteToWideChar(CP_UTF8, 0, tempBuf, -1, wszBuf, sizeof(wszBuf));
		OutputDebugStringW(wszBuf);
		WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, tempBuf, sizeof(tempBuf), nullptr, FALSE);
		printf("%s", tempBuf);

		pos += MAX_LOG_LENGTH;

	} while (pos < len);
	SendLogToWindow(buf);
	fflush(stdout);
	#else
	// Linux, Mac, iOS, etc
	fprintf(stdout, "%s", buf);
	fflush(stdout);
	#endif

	delete[] buf;
}
// FIXME: Deprecated
void CCLog(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    _log(format, args);
    va_end(args);
}

void log(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    _log(format, args);
    va_end(args);
}

