#pragma once
class InPutInterface
{
public:
	virtual void keyInput(unsigned char param, int x, int y)=0;
	virtual void mouseInput(int button, int state, int x, int y)=0;
};