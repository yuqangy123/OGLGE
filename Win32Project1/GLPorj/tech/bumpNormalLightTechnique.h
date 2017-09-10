#pragma once
#include "technique.h"
#include "techDefine.h"




class bumpNormalLightTechnique : public technique
{
public:
	bumpNormalLightTechnique();
	~bumpNormalLightTechnique();

	void init();

public:
	int positionLoc = 0;
	int texCoordLoc = 0;
	int normalLoc = 0;
	int tangentLoc = 0;
	int bitangentLoc = 0;
};

