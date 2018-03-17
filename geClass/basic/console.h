

#ifndef __OGLGE_CONSOLE_H__
#define __OGLGE_CONSOLE_H__
/// @cond DO_NOT_SHOW

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <BaseTsd.h>
#include <WinSock2.h>

#ifndef __SSIZE_T
#define __SSIZE_T
typedef SSIZE_T ssize_t;
#endif // __SSIZE_T

#else
#include <sys/select.h>
#endif

#include <thread>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <mutex>
#include <stdarg.h>
#include <oglgeCommon.h>

/// The max length of CCLog message.
static const int MAX_LOG_LENGTH = 16*1024;

void CCLog(const char * format, ...);

/// @endcond
#endif /* defined(__OGLGE_CONSOLE_H__) */
