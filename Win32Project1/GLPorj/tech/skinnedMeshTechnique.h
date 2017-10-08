#pragma once
#include "technique.h"
#include "techDefine.h"
#include "Pipeline.h"
#include "Camera3D.h"
#include <vector>

class skinnedMeshTechnique : public technique
{
public:
#define MAX_BONES 100

public:
	skinnedMeshTechnique();
	~skinnedMeshTechnique();

	void init();

	void setUniformBonesMat4(unsigned int n, const Matrix4f& mt4);

public:
	int positionLoc ;
	int texCoordLoc ;
	int normalLoc ;
	int weightsLoc ;
	int boneIDsLoc ;
	GLuint m_bonesMat4Loc[MAX_BONES];
};

