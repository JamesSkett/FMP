cbuffer CBuffer0
{
	matrix WVP;           //64 bytes |
	float2 playerPos;     //8 bytes  |
	float playerRotation; //4 bytes  |
	float range;          //4 bytes  | //96 bytes
	int fog;              //4 bytes  |
	float3 packingBytes;  //12 bytes |
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
	if (fog == 1)
	{
		float x = pow((position.x - playerPos.x), 2);
		float y = pow((position.y - playerPos.y), 2);


		float dist = sqrt(x + y);

		return color - float4(dist / 250.f, dist / 250.f, dist / 250.f, 1);
	}

	return color;
}