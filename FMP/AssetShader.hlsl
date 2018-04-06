Texture2D texture0;
SamplerState sampler0;

cbuffer CBuffer0
{
	matrix WVP;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 texcoord :TEXCOORD;
};

VOut VShader(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
	VOut output;

	output.position = mul(WVP, position);

	output.texcoord = texcoord;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return texture0.Sample(sampler0, texcoord);
}