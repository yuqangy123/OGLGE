#pragma once
#include "techDefine.h"
#include "technique.h"



class billboardTechnique : public technique
{
public:
	billboardTechnique();
	~billboardTechnique();

	void init();

public:
	int positionLoc = -1;
};

