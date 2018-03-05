#include <stdlib.h>
#include <android/log.h>
#include <jni.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

extern "C" {

    JNIEXPORT void JNICALL Java_org_game_ge_geRenderer_nativeRender(JNIEnv* env) {
        //cocos2d::Director::getInstance()->mainLoop();
		LOGD("Java_org_game_ge_geRenderer_nativeRender");
    }


}
