#pragma once
#include "technique.h"
#include "techDefine.h"
#include "MeshNode.h"
#include "Pipeline.h"
#include "Camera3D.h"
#include <vector>

class shadowMeshTechnique : public technique
{
public:
	shadowMeshTechnique();
	~shadowMeshTechnique();

	void init();

public:
	int positionLoc = 0;
	int texCoordLoc = 0;
	int normalLoc = 0;
	
};

