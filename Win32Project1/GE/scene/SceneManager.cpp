#include "stdafx.h"
#include "SceneManager.h"

DECLARE_SINGLETON_MEMBER(SceneManager)
SceneManager::SceneManager()
{
	init();
}


SceneManager::~SceneManager()
{

}

void SceneManager::init()
{
	m_scene = new Scene();
}


