/////////////
// GLOBALS //
/////////////
#define LIGHT_DIR float3(5.0, 5.5, -10.0)
#define LIGHT_COLOR float3(0.9, 0.9, 0.9)

Texture2D shaderTexture[2];
SamplerState SampleType;

#define AMBIENT float3(0.0, 0.0, 0.0)
#define SPECULAR float3(1.0, 1.0, 1.0)

#define SPECULARPOWER 100.0

float4 ps_main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD0, float3 normal : NORMAL, float3 tangent : TANGENT, float3 bitangent : BITANGENT, float3 view : VIEW, float4 color : COLOR) : SV_TARGET
{
	float3 L = normalize(LIGHT_DIR);
	L = normalize(view);
	float3 V = normalize(view);
	float3 N = normalize(normal);
	float3 T = normalize(tangent);
	float3 B = normalize(bitangent);
	float3 H = normalize(L + V);

	float4 texColor = shaderTexture[0].Sample(SampleType, texCoord);
	float4 bumpMap = shaderTexture[1].Sample(SampleType, texCoord);

	// expand the range from (0, +1) to (-1, +1)
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	float3 bumpNormal = (-1.0 * bumpMap.x * B) + (-1.0* bumpMap.y * T) + (bumpMap.z * N);

	bumpNormal = normalize(bumpNormal);

	//bumpNormal = N;

	float3 ambient = AMBIENT;
	float3 diffuse = saturate(dot(bumpNormal, L)) * texColor.xyz;
	float3 specular = pow(saturate(dot(bumpNormal, H)), SPECULARPOWER) * SPECULAR;

	float3 outColor = (saturate(ambient + diffuse) + specular) * LIGHT_COLOR;

	return float4(outColor,0.95f);
}
