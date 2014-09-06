#pragma once
#include "Texture.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "util.h"
#include "DirectX11/DDSTextureLoader.h"
#include "DX11Renderer.h"

namespace Transmission {
	class FRAMEWORKDLL DX11Texture : public Texture
	{
	public:
		DX11Texture(DX11Renderer* renderer, const char* filename);
		virtual ~DX11Texture();

		virtual void set(int slot);

	private:
		ID3D11DeviceContext* context;
		ID3D11ShaderResourceView* textureResource;
		ID3D11SamplerState* samplerState;
	};

	DX11Factory(Texture);
}