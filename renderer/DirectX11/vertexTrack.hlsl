cbuffer PerFrame : register(b0)
{
	matrix ViewProjection;
	float3 cameraPosition;
}

cbuffer PerVertex : register(b1)
{
	matrix World;
}

struct VOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float3 view : VIEW;
	float4 color : COLOR;
};

VOut vs_main(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	VOut output;
	
	float4 Pos = float4(position.xyz, 1.0);
	float4 norm = float4(normal.xyz, 0.0);
	float4 tang = float4(tangent.xyz, 0.0);

	float3 bitang = cross(tang, norm);
	bitang = float4(bitang.xyz, 0.0); //not sure if necessary

	output.position = mul(mul(Pos,World),ViewProjection);
	output.texCoord = texCoord;
	output.normal = mul(norm, World).xyz; // this is fine as long as we only do uniform scale
	output.tangent = mul(tang, World).xyz;
	output.bitangent = mul(bitang, World).xyz;
	output.view = (float4(cameraPosition, 1.0) - mul(Pos, World)).xyz;

	//color will be replaced with texture values to be passed to pixel shader

	output.color = float4(1.0,0.0,0.0,1.0);

	return output;
}
