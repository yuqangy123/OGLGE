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



