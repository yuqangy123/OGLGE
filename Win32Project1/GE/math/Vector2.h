#pragma once
#define Vector2f Vector2
struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	~Vector2();

	inline Vector2 operator-(const Vector2& Right)
	{
		Vector2 ret;
		ret.x = (this->x - Right.x);
		ret.y = (this->y - Right.y);
		return ret;
	}
};

