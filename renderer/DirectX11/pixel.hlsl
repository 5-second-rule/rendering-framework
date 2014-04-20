/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

float4 ps_main(float4 position : SV_POSITION, float2 tex: TEXCOORD0) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, tex);
	return textureColor;
}