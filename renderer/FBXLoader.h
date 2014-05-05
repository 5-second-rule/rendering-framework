#pragma once
#include <winerror.h>
#include <vector>

#include "util.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"

namespace Transmission {

	class FRAMEWORKDLL FBXLoader
	{
	public:
		FBXLoader();
		~FBXLoader();

		HRESULT loadFBXFile(char*, VertexBuffer**, IndexBuffer**, Renderer*);
	private:
		std::vector<unsigned int>* indices;
	};

}
