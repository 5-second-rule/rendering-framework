#pragma once
#include "Texture.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "util.h"
#include "DirectX11/DDSTextureLoader.h"


namespace Transmission {
	class DX11Texture :
		public Texture
	{
	private:
		ID3D11DeviceContext* context;
		ID3D11ShaderResourceView* textureResource;
		ID3D11SamplerState* samplerState;

	public:
		DX11Texture(const char*, ID3D11Device*, ID3D11DeviceContext*);
		~DX11Texture();

		virtual void set(int slot);
	};
}