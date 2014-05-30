#define NUM_LIGHTS 10

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
	float4 worldPos : WORLDPOSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float3 view : VIEW;
};

VOut vs_main(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	VOut output;
	
	float4 Pos = float4(position.xyz, 1.0);
	float4 norm = float4(normal.xyz, 0.0);
	float4 tang = float4(tangent.xyz, 0.0);

	float3 bitang = cross(tang.xyz, norm.xyz);

	output.position = mul(mul(Pos,World),ViewProjection);
	output.worldPos = mul(Pos, World);
	output.texCoord = texCoord;
	output.normal = mul(norm, World).xyz; // this is fine as long as we only do uniform scale
	output.tangent = mul(tang, World).xyz;
	output.bitangent = mul(float4(bitang,0.0), World).xyz;
	output.view = (float4(cameraPosition, 1.0) - mul(Pos, World)).xyz;

	return output;
}
