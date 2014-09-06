#include "DX11Texture.h"

#include "dxerr.h"
#include <stdexcept>

namespace Transmission {
	DX11Texture::DX11Texture(DX11Renderer* renderer, const char* textureFile)
		: Texture(), context(renderer->getContext())
	{
		wchar_t news[MAX_PATH];
		mbstowcs(news, textureFile, strlen(textureFile) + 1);

		HR(DirectX::CreateDDSTextureFromFile(
			renderer->getDevice(), news, nullptr, &textureResource
		));

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HR(renderer->getDevice()->CreateSamplerState(&sampDesc, &this->samplerState));
	}


	DX11Texture::~DX11Texture(){
		this->samplerState->Release();
	}

	void DX11Texture::set(int slot)
	{
		context->PSSetShaderResources(slot, 1, &textureResource);
		context->PSSetSamplers(0, 1, &samplerState);
	}
}