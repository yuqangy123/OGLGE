#pragma once
#include "assert.h"


/**Util macro for conversion from degrees to radians.*/
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
/**Util macro for conversion from radians to degrees.*/
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)

#define  PI  (3.1415926)

#define MATH_TOLERANCE              2e-37f
#define MATH_EPSILON                0.000001f

#define DO do{
#define WHILE }while(false);
#define IF_BREAK(ass) if(ass)break;



#define VTC3_REF_VTC4(vtc4) Vector3f(vtc4.x, vtc4.y, vtc4.z)
#define VTC4_TO_VTC3(vtc4, vtc3) {vtc3.x=vtc4.x;vtc3.y=vtc4.y;vtc3.z=vtc4.z;}

#define VTC4_REF_VTC3(vtc3) Vector4f(vtc3.x, vtc3.y, vtc3.z)
#define VTC3_TO_VTC4(vtc3, vtc4) {vtc4.x=vtc3.x;vtc4.y=vtc3.y;vtc4.z=vtc3.z;vtc4.w=1.0f;}


//define namespace micro
#define NS_UISYSTEM_BEGIN                     namespace uiSystem {
#define NS_UISYSTEM_END                       }
#define USING_NS_UISYSTEM                     using namespace uiSystem

#define NS_STRUCT_BEGIN                     namespace OGLGEStruct {
#define NS_STRUCT_END                       }
#define USING_NS_STRUCT                     using namespace OGLGEStruct


#define uint unsigned int