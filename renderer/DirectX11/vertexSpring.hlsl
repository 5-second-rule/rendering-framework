cbuffer PerFrame : register(b0)
{
	matrix ViewProjection;
	float3 cameraPosition;
}

cbuffer PerVertex : register(b1)
{
	matrix World;
}

cbuffer Time : register(b2)
{
	float elapsedTime;
	float timeSinceLastFrame;
}

struct VOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 view : TEXCOORD2;
	float4 color : COLOR;
};

#define M_PI 3.1415926535897932384626433832795

VOut vs_main(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	VOut output;

	float amplitude = 0.2; //0.1
	float period = 1.0;
	float frequency = 1.0 / period;


	//float offset = position.x + position.y - position.z;
	float height = amplitude * sin(2.0*M_PI*frequency*elapsedTime);// +offset);

	float4 Pos = position + float4(0.0, height*position.y, 0.0, 0.0);
	Pos = float4(Pos.xyz, 1.0);

	output.position = mul(mul(Pos,World),ViewProjection);
	output.texCoord = texCoord;
	output.normal = mul(float4(normal.xyz,0.0), World).xyz; // this is fine as long as we only do uniform scale
	output.view = (float4(cameraPosition, 1.0) - mul(Pos, World)).xyz;

	//color will be replaced with texture values to be passed to pixel shader

	output.color = float4(1.0,0.0,0.0,1.0);

	return output;
}
