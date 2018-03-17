LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := oglge_shared

#use to add __android_log_print
LOCAL_MODULE_FILENAME := liboglge

LOCAL_C_INCLUDES  += system/core/include/cutils
LOCAL_SHARED_LIBRARIES := libcutils

# to add android gles and other libs
LOCAL_LDLIBS := -lGLESv1_CM \
			   -lGLESv2 \
			   -lEGL \
			   -llog \
			   -landroid
			   
LOCAL_SRC_FILES := \
java_activity-android.cpp\
Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp \
../../../geClass/basic/base64.cpp \
../../../geClass/basic/ccUTF8.cpp \
../../../geClass/basic/console.cpp \
../../../geClass/external/ConvertUTF/ConvertUTF.cpp \
../../../geClass/external/ConvertUTF/ConvertUTFWrapper.cpp \
../../../geClass/platform/android/JniHelper.cpp \
main.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../geClass \
					$(LOCAL_PATH)/../../../Win32Project1/GE \
					$(LOCAL_PATH)/../../../geClass/platform \
					$(LOCAL_PATH)/../../../geClass/platform/android \
					$(LOCAL_PATH)/../../../geClass/external

					
					   
# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

#LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
#LOCAL_STATIC_LIBRARIES += cocos2d_simulator_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

#$(call import-module,scripting/lua-bindings/proj.android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
