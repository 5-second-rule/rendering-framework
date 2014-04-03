cbuffer cbPSPerFrame
{
	float4 g_vLightPos;
};


Texture2D g_Tex;
SamplerState g_Sampler;


struct PSInput
{
	float4 Pos : SV_Position;
	float3 Normal : NORMAL;
	float2 Texcoord : TEXCOORD0;
	float4 WorldPos : POSITION;
};


float4 ps_main(PSInput In) : SV_Target
{
	float4 Color = g_Tex.Sample(g_Sampler, In.Texcoord);

	float3 vRay = (float3)(g_vLightPos - In.WorldPos);

	float fDot = max(0.0f, dot(normalize(vRay), normalize(In.Normal)));

	return Color * fDot;
}
