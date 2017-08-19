#pragma once
#include "Mat4.h"


class Camera
{
public:
	virtual Matrix4f getCameraTranlation() {return Matrix4f();};
	inline bool dirty() { return m_dirty; };
	inline void dirty(bool dir) { m_dirty = dir; };

protected:
	bool		m_dirty=false;
};

