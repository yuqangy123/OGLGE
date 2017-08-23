#pragma once
#include "technique.h"
#include "techDefine.h"


#define ATTRI_UNIFORM_DIRLIGHT_color "gDirectionLight.color"
#define ATTRI_UNIFORM_DIRLIGHT_ambientIntensity "gDirectionLight.ambientIntensity"
#define ATTRI_UNIFORM_DIRLIGHT_direction "gDirectionLight.direction"
#define ATTRI_UNIFORM_DIRLIGHT_diffuseIntensity "gDirectionLight.diffuseIntensity"

typedef enum{ 
	ambientLight = 1,
	diffuseLight = 2,
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
	int lightColorLoc = 0;
	int lightAmbientIntensityLoc = 0;
	int lightDirectionLoc = 0;
	int lightDiffuseIntensityLoc = 0;
};

