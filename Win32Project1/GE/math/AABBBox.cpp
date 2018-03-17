

#include "AABBBox.h"


AABBBox::AABBBox()
{
}
AABBBox::AABBBox(const Vector3f& minPosition, const Vector3f& maxPosition)
{
	min = minPosition;
	max = maxPosition;
}