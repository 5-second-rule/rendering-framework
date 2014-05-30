/////////////
// GLOBALS //
/////////////
#define NUM_LIGHTS 10

#define LIGHT_DIR float3(5.0, 5.5, -10.0)
#define LIGHT_COLOR float3(0.9, 0.9, 0.9)

Texture2D shaderTexture;
SamplerState SampleType;

#define AMBIENT float3(0.0, 0.0, 0.0)
#define SPECULAR float3(1.0, 1.0, 1.0)

#define SPECULARPOWER 100.0

struct LightData
{
	float4 lightPosition;
	float4 lightColor;
};

cbuffer LightDataBuffer : register(b0)
{
	LightData lightDataArray[NUM_LIGHTS];
};

float4 ps_main(float4 position : SV_POSITION, float4 worldPos : WORLDPOSITION, float2 texCoord : TEXCOORD0, float3 normal : TEXCOORD1, float3 view : TEXCOORD2) : SV_TARGET
{
	float3 L = normalize(LIGHT_DIR);
	float3 V = normalize(view);
	float3 N = normalize(normal);
	float3 H = normalize(L + V);

	float edgeDetector = saturate(dot(N, V));
	float diffuseFactor = saturate(dot(N, L));

	if (diffuseFactor < 0.1) {
		diffuseFactor = 0.0;
	} else if (diffuseFactor < 0.3) {
		diffuseFactor = 0.3;
	} else if (diffuseFactor < 0.6) {
		diffuseFactor = 0.6;
	} else {
		diffuseFactor = 1.0;
	}

	if (edgeDetector < 0.2) {
		diffuseFactor = 0.0;
	}

	float4 texColor = shaderTexture.Sample(SampleType, texCoord);

	float3 ambient = AMBIENT;
	float3 diffuse = diffuseFactor * texColor.xyz;
	float3 specular = pow(saturate(dot(N, H)), SPECULARPOWER) * SPECULAR;

	float3 outColor = (saturate(ambient + diffuse)) * LIGHT_COLOR;

	return float4(outColor,1.0f);
}
