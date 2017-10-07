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
	int positionLoc;
	int texCoordLoc;
	int normalLoc;
	
};

