
#include "JniHelper.h"
#include <jni.h>
#include <platformConfig.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace ns_oglge;

void oglge_android_app_init(JNIEnv* env) {
    LOGD("oglge_android_app_init");
   
}

extern "C"
{
    bool Java_org_cocos2dx_lua_AppActivity_nativeIsLandScape(JNIEnv *env, jobject thisz)
    {
		LOGD("Java_org_cocos2dx_lua_AppActivity_nativeIsLandScape");
        return false;
    }


}

