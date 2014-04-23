#define LIGHT_DIR float3(10.0, 10.0, 0.0)
#define LIGHT_COLOR float3(0.9, 0.9, 0.9)

#define AMBIENT float3(0.1, 0.1, 0.1)
#define DIFFUSE float3(0.7, 0.5, 0.5)
#define SPECULAR float3(0.5, 0.5, 0.5)

float SPECULARPOWER = 0.3;

float4 ps_main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD0, float3 normal : TEXCOORD1, float3 view : TEXCOORD2, float4 color : COLOR) : SV_TARGET
{
	float3 L = normalize(LIGHT_DIR);
	float3 V = normalize(view);
	float3 N = normalize(normal);
	float3 H = normalize(L + V);

	float3 ambient = AMBIENT;
	float3 diffuse = saturate(dot(N, L)) * DIFFUSE;
	float3 specular = pow(saturate(dot(N, H)), SPECULARPOWER) * SPECULAR;

	float4 texColor = color; // tex2D(sampTexture, texCoord);

	float3 outColor = (saturate(ambient + diffuse) * texColor.xyz + specular) * LIGHT_COLOR;

	return float4(outColor,1.0f);
}