#pragma once
#include "techDefine.h"
#include "technique.h"



class normalTechnique : public technique
{
public:
	normalTechnique();
	~normalTechnique();

	void init();

public:
	int positionLoc = 0;
	int texCoordLoc = 0;
	int colorLoc = 0;
};

