/////////////
// GLOBALS //
/////////////
#define NUM_LIGHTS 10
#define CAMERA_RANGE 1000

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
	//first light is the camera
	float lightIntensity;
	float attenuation;
	attenuation = saturate(1 - dot(view / CAMERA_RANGE, view / CAMERA_RANGE));
	attenuation = pow(attenuation, 2);

	float3 L = normalize(view);
		float3 V = normalize(view);
		float3 N = normalize(normal);
		float3 H = normalize(L + V);

		float4 texColor = shaderTexture.Sample(SampleType, texCoord);

		float3 color = AMBIENT;

		lightIntensity = saturate(dot(N, L));

	if (lightIntensity > 0)
	{
		color += LIGHT_COLOR * lightIntensity * attenuation;
		color = saturate(color);
	}

	float3 lightDirection;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		if (lightDataArray[i].lightPosition[3] != 0)
		{
			lightDirection = (lightDataArray[i].lightPosition - worldPos).xyz;
			attenuation = saturate(1 - dot(lightDirection / lightDataArray[i].lightColor.w, lightDirection / lightDataArray[i].lightColor.w));
			attenuation = pow(attenuation, 2);

			if (attenuation > 0)
			{
				L = normalize(lightDirection);
				H = normalize(L + V);

				lightIntensity = saturate(dot(N, L));

				if (lightIntensity > 0)
				{
					color += lightDataArray[i].lightColor.xyz * lightIntensity * attenuation;
					color = saturate(color);
				}
			}
		}
	}

	color = color * texColor.xyz;

	float3 outColor = saturate(color);

	return float4(outColor, 1.0f);
}
