#pragma once
#include "technique.h"
#include "techDefine.h"


typedef enum{ 
	ambientLight = 1,
	diffuseLight = 2,
	specularLight = 3,
}lightType;

class lightTechnique : public technique
{
public:
	lightTechnique();
	~lightTechnique();

	void init(lightType tp);

public:
	int positionLoc = 0;
	int texCoordLoc = 0;
	int normalLoc = 0;
	
};

