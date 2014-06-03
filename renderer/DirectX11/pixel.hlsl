/////////////
// GLOBALS //
/////////////
#define NUM_LIGHTS 10
#define CAMERA_RANGE 1000

#define LIGHT_DIR float3(5.0, 5.5, -10.0)
#define LIGHT_COLOR float3(0.9, 0.9, 0.9)

Texture2D shaderTexture;
SamplerState SampleType;

#define AMBIENT float3(0.3, 0.3, 0.3)
#define SPECULAR float3(0.3, 0.3, 0.3)

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

cbuffer SaturationLightnessBuffer : register(b1)
{
	float conSaturation;
	float conLightness;
};

//RGB conversion functions
float Epsilon = 1e-10;

float3 HUEtoRGB(in float H)
{
	float R = abs(H * 6 - 3) - 1;
	float G = 2 - abs(H * 6 - 2);
	float B = 2 - abs(H * 6 - 4);
	return saturate(float3(R, G, B));
}

float3 RGBtoHCV(in float3 RGB)
{
	// Based on work by Sam Hocevar and Emil Persson
	float4 P = (RGB.g < RGB.b) ? float4(RGB.bg, -1.0, 2.0 / 3.0) : float4(RGB.gb, 0.0, -1.0 / 3.0);
		float4 Q = (RGB.r < P.x) ? float4(P.xyw, RGB.r) : float4(RGB.r, P.yzx);
		float C = Q.x - min(Q.w, Q.y);
	float H = abs((Q.w - Q.y) / (6 * C + Epsilon) + Q.z);
	return float3(H, C, Q.x);
}


float3 RGBtoHSL(in float3 RGB)
{
	float3 HCV = RGBtoHCV(RGB);
		float L = HCV.z - HCV.y * 0.5;
	float S = HCV.y / (1 - abs(L * 2 - 1) + Epsilon);
	return float3(HCV.x, S, L);
}

float3 HSLtoRGB(in float3 HSL)
{
	float3 RGB = HUEtoRGB(HSL.x);
		float C = (1 - abs(2 * HSL.z - 1)) * HSL.y;
	return (RGB - 0.5) * C + HSL.z;
}
//

float4 ps_main(float4 position : SV_POSITION, float4 worldPos : WORLDPOSITION, float2 texCoord : TEXCOORD0, float3 normal : TEXCOORD1, float3 view : TEXCOORD2) : SV_TARGET
{
	//first light is the camera
	float lightIntensity;
	float attenuation;
	attenuation = saturate(1 - dot(view /CAMERA_RANGE , view / CAMERA_RANGE));
	attenuation = pow(attenuation, 2);

	float3 L = normalize(view);
	float3 V = normalize(view);
	float3 N = normalize(normal);
	float3 H = normalize(L + V);

	float4 texColor = shaderTexture.Sample(SampleType, texCoord);

	float3 color = AMBIENT;
	float3 specular = float3(0, 0, 0);

	lightIntensity = saturate(dot(N, L));
	lightIntensity *= 0.5;

	if (lightIntensity > 0)
	{
		color += LIGHT_COLOR * lightIntensity * attenuation;
		color = saturate(color);

		specular += pow(saturate(dot(N, H)), SPECULARPOWER) * SPECULAR * lightIntensity * attenuation;
		specular = saturate(specular);
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

					specular += pow(saturate(dot(N, H)), SPECULARPOWER) * SPECULAR * lightIntensity * attenuation;
					specular = saturate(specular);
				}
			}
		}
	}

	color = color * texColor.xyz;

	float3 outColor = saturate(color + specular);

	float3 HSLColor = RGBtoHSL(outColor);

	//Saturation Change
	HSLColor.y = HSLColor.y + conSaturation;

	//Lightness CHange
	HSLColor.z = HSLColor.z + conLightness;

	outColor = HSLtoRGB(HSLColor);

	return float4(outColor,1.0f);
}
