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
../../../geClass/basic/*.cpp \
../../../geClass/external/ConvertUTF/ConvertUTF.cpp \
../../../geClass/external/ConvertUTF/ConvertUTFWrapper.cpp \
../../../geClass/platform/android/JniHelper.cpp \
../../../Win32Project1/application/*.cpp \
../../../Win32Project1/basic/*.cpp \
../../../Win32Project1/GE/*.cpp \
../../../Win32Project1/GE/math/*.cpp \
../../../Win32Project1/GE/renderer/*.cpp \
../../../Win32Project1/GE/scene/*.cpp \
../../../Win32Project1/GE/struct/*.cpp \
../../../Win32Project1/GE/GLPorj/*.cpp \

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
