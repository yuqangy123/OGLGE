#pragma once
#include "technique.h"
#include "techDefine.h"


typedef enum{ 
	ambientLight = 1,
	diffuseLight = 2,
	specularLight = 3,
	dotDiffuseLight = 4,
}lightType;

class lightTechnique : public technique
{
public:
	lightTechnique();
	~lightTechnique();

	void init(lightType tp);

public:
	int positionLoc;
	int texCoordLoc;
	int normalLoc;
	
};

