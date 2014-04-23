#pragma once
#include <winerror.h>

#include "util.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"

namespace Transmission {

	class OBJLoader
	{
	public:
		OBJLoader();
		~OBJLoader();

		HRESULT loadOBJFile(char* filePath, VertexBuffer** vBuf, IndexBuffer** iBuf, Renderer* renderer);
		bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
		bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, VertexBuffer** vBuf, IndexBuffer** iBuf, Renderer* renderer);
	};

}