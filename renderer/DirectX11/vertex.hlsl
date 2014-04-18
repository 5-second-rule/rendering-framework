cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut vs_main(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL)
{
	VOut output;

	output.position = mul(mul(mul(position,World),View),Projection);

	//add texture lookup here

	output.color = float4(1.0,0.0,0.0,1.0);

	return output;
}
