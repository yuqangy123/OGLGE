#pragma once
#include "techDefine.h"
#include "technique.h"



class skyBoxTechnique : public technique
{
public:
	skyBoxTechnique();
	~skyBoxTechnique();

	void init();

public:
	int positionLoc;
};

