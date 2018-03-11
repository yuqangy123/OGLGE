

#include <stdlib.h>
#include <android/log.h>
#include <jni.h>
#include <JniHelper.h>

#include <GL-android.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

void oglge_android_app_init(JNIEnv* env) __attribute__((weak));

using namespace ns_oglge;

extern "C"
{

	JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
	{
		LOGD("JNI_OnLoad");
		JniHelper::setJavaVM(vm);

		oglge_android_app_init(JniHelper::getEnv());

		return JNI_VERSION_1_4;
	}

	JNIEXPORT void Java_org_game_ge_geRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
	{
		glEnable(GL_BLEND);
		LOGD("Java_org_game_ge_geRenderer_nativeInit");
	}
	
	JNIEXPORT void Java_org_game_ge_geRenderer_nativeOnSurfaceChanged(JNIEnv*  env, jobject thiz, jint w, jint h)
	{
		LOGD("Java_org_game_ge_geRenderer_nativeOnSurfaceChanged");
	}
}


