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

cbuffer Time : register(b2)
{
	float elapsedTime;
	float timeSinceLastFrame;
}

struct VOut
{
	float4 position : SV_POSITION;
	float4 worldPos : WORLDPOSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 view : TEXCOORD2;
};

#define M_PI 3.1415926535897932384626433832795

VOut vs_main(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	VOut output;

	float zAmplitude = 0.3;
	float cAmplitude = 0.1;

	float zPeriod = 2.0;
	float zFrequency = 1.0 / zPeriod;

	float cPeriod = 1.0;
	float cFrequency = 1.0 / cPeriod;

	float scale = 0.5 * position.x * position.y;
	float4 diagonal;
	float4 counterDiagonal;

	float zMove = scale * zAmplitude * sin(2.0*M_PI*zFrequency*elapsedTime);
	float curve = scale * cAmplitude * sin(2.0*M_PI*cFrequency*elapsedTime);
	float absCurve = abs(0.2*curve);

	if (position.x >= 0.0) {
		if (position.y >= 0.0){
			diagonal = float4(-1.0, 1.0, 0.0, 0.0);
			counterDiagonal = float4(-1.0, -1.0, 0.0, 0.0);
		}
		else {
			diagonal = float4(-1.0, -1.0, 0.0, 0.0);
			counterDiagonal = float4(-1.0, 1.0, 0.0, 0.0);
		}
	}
	else {
		if (position.y >= 0.0){
			diagonal = float4(1.0, 1.0, 0.0, 0.0);
			counterDiagonal = float4(1.0, -1.0, 0.0, 0.0);
		}
		else {
			diagonal = float4(1.0, -1.0, 0.0, 0.0);
			counterDiagonal = float4(1.0, 1.0, 0.0, 0.0);
		}
	}


	float4 Pos = position + float4(0.0, 0.0, zMove*1.0, 0.0) + curve*diagonal + absCurve*counterDiagonal;
	Pos = float4(Pos.xyz, 1.0);

	output.position = mul(mul(Pos,World),ViewProjection);
	output.worldPos = mul(Pos, World);
	output.texCoord = texCoord;
	output.normal = mul(float4(normal.xyz,0.0), World).xyz; // this is fine as long as we only do uniform scale
	output.view = (float4(cameraPosition, 1.0) - mul(Pos, World)).xyz;

	return output;
}
