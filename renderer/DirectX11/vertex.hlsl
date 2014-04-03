cbuffer cbVSPerFrame
{
	matrix g_mWorld;
	matrix g_mViewProj;
};


struct VSInput
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Texcoord : TEXCOORD0;
};

struct PSInput
{
	float4 Pos : SV_Position;
	float3 Normal : NORMAL;
	float2 Texcoord : TEXCOORD0;
	float4 WorldPos : POSITION;
};


PSInput vs_main(VSInput In)
{
	PSInput Out;

	Out.Normal = mul(In.Normal, (float3x3)g_mWorld);
	Out.WorldPos = mul(In.Pos, g_mWorld);
	Out.Pos = mul(Out.WorldPos, g_mViewProj);
	Out.Texcoord = In.Texcoord;

	return Out;
}
