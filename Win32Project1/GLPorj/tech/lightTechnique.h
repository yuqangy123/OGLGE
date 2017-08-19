#pragma once
#include "technique.h"

#define ATTRI_A_POSITION_INDEX	0
#define ATTRI_A_POSITION_NAME		"a_position"
#define ATTRI_A_COLOR_INDEX	1
#define ATTRI_A_COLOR_NAME		"Color"
#define ATTRI_A_TEXCOORD_INDEX	2
#define ATTRI_A_TEXCOORD_NAME		"a_texCoord"

class lightTechnique : public technique
{
public:
	lightTechnique();
	~lightTechnique();

	void init();

public:
	int positionLoc = 0;
	int texCoordLoc = 0;
	int colorLoc = 0;
};

