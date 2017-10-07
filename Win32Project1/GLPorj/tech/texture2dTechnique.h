#pragma once
#include "techDefine.h"
#include "technique.h"





class texture2dTechnique : public technique
{
public:
	texture2dTechnique();
	~texture2dTechnique();

	void init();

public:
	int positionLoc;
	int texCoordLoc;
};

