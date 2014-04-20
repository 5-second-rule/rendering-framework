cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VOut
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VOut vs_main(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL)
{
	VOut output;

	output.position = mul(mul(mul(position,World),View),Projection);

	//color will be replaced with texture values to be passed to pixel shader

	output.tex = texCoord;

	return output;
}
