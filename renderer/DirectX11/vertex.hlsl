cbuffer ConstantBuffer : register(b0)
{
	matrix View;
	matrix Projection;
}

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut vs_main(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = mul(mul(position,View),Projection);
	output.color = color;

	return output;
}