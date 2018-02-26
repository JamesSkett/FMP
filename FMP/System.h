#pragma once
#include "Renderer.h"
class System
{
public:
	System();
	~System();

	int Run(MSG msg, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

private:
	Renderer* renderer;
};

