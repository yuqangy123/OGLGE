#pragma once
#include "technique.h"
#include "techDefine.h"
#include "MeshNode.h"
#include "Pipeline.h"
#include "Camera3D.h"
#include <vector>

class shadowMapMeshTechnique : public technique
{
public:
	shadowMapMeshTechnique();
	~shadowMapMeshTechnique();

	void init();

	void setTargetPosition(float x, float y, float z);
	void setLightPosition(float x, float y, float z);

	const Vector3& getLightPosition() { return m_pLightSpot->getEyePosition(); };
	const Vector3& getTargetPosition() { return m_pLightSpot->getTargetPosition(); };
	const Matrix4f* getLightWorldTrans() { return m_pipe.GetWorldTrans(); }

	void renderFrameBuffer(std::vector<MeshNode*>&);


protected:
	void initFBO();

public:
	int positionLoc = 0;
	int texCoordLoc = 0;
	
	GLuint fbo = 0;
	GLuint shadowMap = 0;

protected:
	Pipeline m_pipe;
	Camera3D* m_pLightSpot = nullptr;
	Matrix4f m_lightMVPMt4;
};

