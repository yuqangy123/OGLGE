#pragma once
#include "Vector3.h"
#include "AABBBox.h"

class MeshNode
{
public:
	typedef struct boundBoxData_{
		Vector3f left_top_front;
		Vector3f right_top_front;
		Vector3f right_bottom_front;
		Vector3f left_bottom_front;
		Vector3f left_top_back;
		Vector3f right_top_back;
		Vector3f right_bottom_back;
		Vector3f left_bottom_back;
		AABBBox boundBox;


		void init(float maxx, float minx, float maxy, float miny, float maxz, float minz)
		{
			left_top_front = Vector3f(minx, maxy,maxz);
			right_top_front = Vector3f(maxx, maxy,maxz);
			right_bottom_front = Vector3f(maxx, miny, maxz);
			left_bottom_front = Vector3f(minx,miny,maxz);
			left_top_back = Vector3f(minx, maxy, minz);
			right_top_back = Vector3f(maxx, maxy, minz);
			right_bottom_back = Vector3f (maxx, miny, minz);
			left_bottom_back = Vector3f (minx, miny, minz);

			boundBox.max = Vector3f(maxx, maxy, maxz);
			boundBox.min = Vector3f(minx, miny, minz);
		}
		boundBoxData_()
		{
		}
		
	}boundBoxData;
};
