#pragma once
#include "techDefine.h"
#include "technique.h"



class normalTexTechnique : public technique
{
public:
	normalTexTechnique();
	~normalTexTechnique();

	void init();

public:
	int positionLoc;
	int texCoordLoc;
};

