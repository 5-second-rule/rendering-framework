/////////////
// GLOBALS //
/////////////

Texture2D shaderTexture;
SamplerState SampleType;

float4 ps_main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD0, float3 normal : TEXCOORD1, float3 view : TEXCOORD2, float4 color : COLOR) : SV_TARGET
{
	float4 texColor = shaderTexture.Sample(SampleType, texCoord);
	return texColor;
}
