#pragma once
#include "Singleton.h"
#include "Node.h"
#include "Scene.h"
#include <vector>

#define SceneManagerIns SceneManager::InstanceEx()
#define DefaultSceneIns (*SceneManagerIns.getRenderScene())
class SceneManager : public CSingleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	Scene* getRenderScene() { return m_renderScene; };

	void update(float ft) { m_renderScene->update(ft); };

	void draw() { m_renderScene->draw(); }

protected:
	void init();

protected:
	Scene* m_renderScene;
};

