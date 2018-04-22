#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

class Math
{
public:
	Math();
	~Math();
	static float Dot(XMFLOAT2 xy0, XMFLOAT2 xy1);
	static float Distance(XMFLOAT2 point1, XMFLOAT2 point2);
};

