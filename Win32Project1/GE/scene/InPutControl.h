#pragma once
#include "Singleton.h"
#include "InPutInterface.h"
#include <vector>

#define InPutControlIns InPutControl::InstanceEx()
class InPutControl : public CSingleton<InPutControl> , public InPutInterface
{
public:
	InPutControl();
	~InPutControl();

	void addListenNode(InPutInterface* nd);
	void removeListenNode(InPutInterface* nd);

	void keyInput(unsigned char param, int x, int y);
	void mouseInput(int button, int state, int x, int y);

protected:
	void init();

protected:
	std::vector<InPutInterface*> m_nodes;
};

