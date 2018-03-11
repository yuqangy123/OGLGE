

#ifndef __BASE_PLATFORM_CONFIG_H__
#define __BASE_PLATFORM_CONFIG_H__



#define OGLGE_PLATFORM_UNKNOWN            0
#define OGLGE_PLATFORM_IOS                1
#define OGLGE_PLATFORM_ANDROID            2
#define OGLGE_PLATFORM_WIN32              3
#define OGLGE_PLATFORM_MARMALADE          4
#define OGLGE_PLATFORM_LINUX              5
#define OGLGE_PLATFORM_BADA               6
#define OGLGE_PLATFORM_BLACKBERRY         7
#define OGLGE_PLATFORM_MAC                8
#define OGLGE_PLATFORM_NACL               9
#define OGLGE_PLATFORM_EMSCRIPTEN        10
#define OGLGE_PLATFORM_TIZEN             11
#define OGLGE_PLATFORM_QT5               12
#define OGLGE_PLATFORM_WINRT             13

// Determine target platform by compile environment macro.
#define OGLGE_TARGET_PLATFORM             OGLGE_PLATFORM_UNKNOWN

// Apple: Mac and iOS
#if defined(__APPLE__) && !defined(ANDROID) // exclude android for binding generator.
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
        #undef  OGLGE_TARGET_PLATFORM
        #define OGLGE_TARGET_PLATFORM         OGLGE_PLATFORM_IOS
    #elif TARGET_OS_MAC
        #undef  OGLGE_TARGET_PLATFORM
        #define OGLGE_TARGET_PLATFORM         OGLGE_PLATFORM_MAC
    #endif
#endif

// android
#if defined(ANDROID)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM         OGLGE_PLATFORM_ANDROID
#endif

// win32
#if defined(_WIN32) && defined(_WINDOWS)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM         OGLGE_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM         OGLGE_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  OGLGE_TARGET_PLATFORM
#define OGLGE_TARGET_PLATFORM         OGLGE_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  OGLGE_TARGET_PLATFORM
#define OGLGE_TARGET_PLATFORM         OGLGE_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM     OGLGE_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM     OGLGE_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM     OGLGE_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM     OGLGE_PLATFORM_TIZEN
#endif

// qt5
#if defined(CC_TARGET_QT5)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM     OGLGE_PLATFORM_QT5
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
    #undef  OGLGE_TARGET_PLATFORM
    #define OGLGE_TARGET_PLATFORM          OGLGE_PLATFORM_WINRT
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! OGLGE_TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if (OGLGE_TARGET_PLATFORM == OGLGE_PLATFORM_WIN32)
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif
#endif  // OGLGE_PLATFORM_WIN32



/** @def CC_DEPRECATED_ATTRIBUTE
* Only certain compilers support __attribute__((deprecated)).
*/
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define OGLGE_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
#define OGLGE_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
#define OGLGE_DEPRECATED_ATTRIBUTE
#endif 



/** @def CC_FORMAT_PRINTF(formatPos, argPos)
* Only certain compiler support __attribute__((format))
*
* @param formatPos 1-based position of format string argument.
* @param argPos    1-based position of first format-dependent argument.
*/
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define OGLGE_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#elif defined(__has_attribute)
#if __has_attribute(format)
#define OGLGE_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#endif // __has_attribute(format)
#else
#define OGLGE_FORMAT_PRINTF(formatPos, argPos)
#endif




#if (OGLGE_TARGET_PLATFORM == OGLGE_PLATFORM_ANDROID)
#include <platformDefine_android.h>
#include <android/log.h>
#endif  // OGLGE_PLATFORM_ANDROID

/// @endcond
#endif  // __BASE_PLATFORM_CONFIG_H__
