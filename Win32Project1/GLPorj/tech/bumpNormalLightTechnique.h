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
	int positionLoc ;
	int texCoordLoc ;
	int normalLoc ;
	int tangentLoc ;
	int bitangentLoc ;
};

