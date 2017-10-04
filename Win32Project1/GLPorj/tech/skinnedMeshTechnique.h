#pragma once
#include "technique.h"
#include "techDefine.h"
#include "MeshNode.h"
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
	int positionLoc = 0;
	int texCoordLoc = 0;
	int normalLoc = 0;
	int weightsLoc = 0;
	int boneIDsLoc = 0;
	GLuint m_bonesMat4Loc[MAX_BONES];
};

