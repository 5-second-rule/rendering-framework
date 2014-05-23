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

#define X_MID 0.7
#define Y_MID -0.1
#define Z_MID -0.3

VOut vs_main(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	VOut output;

	float cAmplitude = 0;

	float cPeriod = 0;
	float cFrequency = 0;

	float offset = 0;

	float scale = 0;
	float4 diagonal;
	
	float curve;

	float4 Pos;
	if (position.y < Y_MID) {

		scale = 1.5 * (position.y - Y_MID);

		if (position.x >= X_MID) {
			if (position.z >= Z_MID){
				diagonal = float4(1.0, 0.0, 1.0, 0.0);
				
				cAmplitude = 0.1;
				cPeriod = 0.7;
				cFrequency = 1.0 / cPeriod;

				offset = position.y;
			}
			else {
				diagonal = float4(1.0, 0.0, -1.0, 0.0);

				cAmplitude = 0.1;
				cPeriod = 0.7;
				cFrequency = 1.0 / cPeriod;

				offset = position.y;
			}
		}
		else {
			if (position.z >= Z_MID){
				diagonal = float4(-1.0, 0.0, 1.0, 0.0);

				cAmplitude = 0.1;
				cPeriod = 0.4;
				cFrequency = 1.0 / cPeriod;

				offset = position.y;
			}
			else {
				diagonal = float4(-1.0, 0.0, -1.0, 0.0);

				cAmplitude = 0.1;
				cPeriod = 0.7;
				cFrequency = 1.0 / cPeriod;

				offset = position.y;
			}
		}

		curve = scale * cAmplitude * sin(2.0*M_PI*cFrequency*elapsedTime + offset);
	}
	else {
		diagonal = float4(normal.xyz, 0.0);

		scale = 1.0;

		cAmplitude = 0.1;
		cPeriod = 1.0;
		cFrequency = 1.0 / cPeriod;

		curve = scale * cAmplitude * sin(2.0*M_PI*cFrequency*elapsedTime);
	}

	Pos = position + curve*diagonal;

	Pos = float4(Pos.xyz, 1.0);

	output.position = mul(mul(Pos,World),ViewProjection);
	output.texCoord = texCoord;
	output.normal = mul(float4(normal.xyz,0.0), World).xyz; // this is fine as long as we only do uniform scale
	output.view = (float4(cameraPosition, 1.0) - mul(Pos, World)).xyz;

	//color will be replaced with texture values to be passed to pixel shader

	output.color = float4(1.0,0.0,0.0,1.0);

	return output;
}
