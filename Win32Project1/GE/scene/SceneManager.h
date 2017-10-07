#pragma once
#include "Singleton.h"
#include "Node.h"
#include "Scene.h"
#include <vector>

#define SceneManagerIns SceneManager::InstanceEx()
#define SceneIns (*SceneManagerIns.getScene())
class SceneManager : public CSingleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	Scene* getScene() { return m_scene; };

	void update(float ft) { m_scene->update(ft); };

	void draw() { m_scene->draw(); }

protected:
	void init();

protected:
	Scene* m_scene;
};

