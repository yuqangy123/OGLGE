#pragma once



class Mat3
{
public:
	Mat3();
	Mat3(float f00, float f01, float f02, float f10, float f11, float f12, float f20, float f21, float f22);
	~Mat3();
	void zero();

public:
	float m[3][3];

};



