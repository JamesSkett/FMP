#include "Math.h"



Math::Math()
{
}


Math::~Math()
{
}

float Math::Dot(XMFLOAT2 xy0, XMFLOAT2 xy1)
{
	return (xy0.x * xy1.x) + (xy0.y * xy1.y);
}

float Math::Distance(XMFLOAT2 point1, XMFLOAT2 point2)
{
	return sqrt((point2.x - point1.x) * (point2.x - point1.x) + (point2.y - point1.y) * (point2.y - point1.y));
}