#pragma once
#include "techDefine.h"
#include "technique.h"



class GBufferDiffuselightTechnique : public technique
{
public:
	GBufferDiffuselightTechnique();
	~GBufferDiffuselightTechnique();

	void init();

public:
	int positionLoc;
};

