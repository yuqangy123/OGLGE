#pragma once
#include "techDefine.h"
#include "technique.h"



class geometryTechnique : public technique
{
public:
	geometryTechnique();
	~geometryTechnique();

	void init();

public:
	int positionLoc;
	int texCoordLoc;
	int normalLoc;
};

