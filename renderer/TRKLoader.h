#pragma once
#include <winerror.h>

#include "util.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"

namespace Transmission {

	class TRKLoader
	{
	public:
		TRKLoader();
		~TRKLoader();

		HRESULT loadTRKFile(char* filePath, VertexBuffer** vBuf, IndexBuffer** iBuf, Renderer* renderer);
		bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& tangentCount, int& faceCount);
		bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int tangentCount, int faceCount, VertexBuffer** vBuf, IndexBuffer** iBuf, Renderer* renderer);
	};

}