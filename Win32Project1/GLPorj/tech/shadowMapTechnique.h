#pragma once
#include "technique.h"
#include "techDefine.h"


class shadowMapTechnique : public technique
{
public:
	shadowMapTechnique();
	~shadowMapTechnique();

	void init();

public:
	int positionLoc = 0;
	int texCoordLoc = 0;
	
};

