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
	const Matrix4f* getLightTrans() { return m_pipe.GetTrans(); }

	void renderFrameBuffer(std::vector<MeshNode*>&);


protected:
	void initFBO();

public:
	int positionLoc ;
	int texCoordLoc ;
	
	GLuint fbo ;
	GLuint shadowMap ;

protected:
	Pipeline m_pipe;
	Camera3D* m_pLightSpot;
	Matrix4f m_lightMVPMt4;
};

