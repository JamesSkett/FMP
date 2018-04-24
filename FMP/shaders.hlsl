cbuffer CBuffer0
{
	matrix WVP;
	float2 playerPos;
	float playerRotation;
	float range;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = mul(WVP, position);
	output.color = color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	//float2 dir = float2(position.x, position.y) - playerPos;

	//float2 playerDir = float2(cos(playerRotation), sin(playerRotation));

	//float angle = acos(dot(playerDir, dir));

	//if (angle >= 50)
	//{
	//	return float4(0, 1, 0, 1);
	//}

	float x = pow((position.x - playerPos.x),2);
	float y = pow((position.y - playerPos.y),2);


	float dist = sqrt(x + y);

	return color  - float4(dist / 300.f, dist / 300.f, dist / 300.f, 1);
	

	return color;
}