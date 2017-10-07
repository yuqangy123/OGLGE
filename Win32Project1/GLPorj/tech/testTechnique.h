#pragma once
#include "techDefine.h"
#include "technique.h"





class testTechnique : public technique
{
public:
	testTechnique();
	~testTechnique();

	void init();

public:
	int positionLoc;
};

