#include "DX11Texture.h"

#include "dxerr.h"
#include <stdexcept>

namespace Transmission {
	DX11Texture::DX11Texture(const char* textureFile, ID3D11Device* device, ID3D11DeviceContext* context) : Texture(textureFile), context(context) {
		wchar_t news[MAX_PATH];
		mbstowcs(news, textureFile, strlen(textureFile) + 1);

		if (FAILED(DirectX::CreateDDSTextureFromFile(device, news, nullptr, &textureResource))) {
			throw std::runtime_error("Couldn't create texture");
		}

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HR(device->CreateSamplerState(&sampDesc, &samplerState));
	}


	DX11Texture::~DX11Texture()
	{
	}

	void DX11Texture::set()
	{
		context->PSSetShaderResources(0, 1, &textureResource);
		context->PSSetSamplers(0, 1, &samplerState);
	}
}