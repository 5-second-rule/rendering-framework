#include "FBXLoader.h"
#include "fbxsdk.h"

#include <Windows.h>
#pragma comment(lib, "libfbxsdk.lib")

#include <cassert>


namespace Transmission {

FbxManager* g_pFbxSdkManager = nullptr;

FBXLoader::FBXLoader()
{
	indices = new std::vector<unsigned int>();
}

FBXLoader::~FBXLoader()
{
	delete indices;
}

HRESULT FBXLoader::loadFBXFile(char* filePath, VertexBuffer** vBuf, IndexBuffer** iBuf, Renderer* renderer)
{
	if (g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(filePath, -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return E_FAIL;

	FbxAxisSystem sceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem DirectXAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eLeftHanded);

	if (sceneAxisSystem != DirectXAxisSystem)
	{
		DirectXAxisSystem.ConvertScene(pFbxScene);
	}

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		// Check if the getChildCount is > 1  TODO
		int test = pFbxRootNode->GetChildCount();

		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();
			//int controlCount = pMesh->GetControlPointsCount();
			int vertexCount = pMesh->GetPolygonCount() * 3; //Note at this point this is the same as numIndices below

			//Vertex vertex;
			Vertex* vertexArray = new Vertex[vertexCount];
			//Vertex vertexArray[2592];

			/*for (int l = 0; l < controlCount; l++)
			{
				vertex.point[0] = (float)pVertices[l].mData[0];
				vertex.point[1] = (float)pVertices[l].mData[1];
				vertex.point[2] = (float)pVertices[l].mData[2];

				vertexArray[l] = vertex;
			}*/

			int numIndices = pMesh->GetPolygonVertexCount();
			//unsigned int* indices = (unsigned int*)pMesh->GetPolygonVertices();
			unsigned int* indexArray = new unsigned int [numIndices];


			FbxVector4 fbxNorm(0, 0, 0, 0);
			FbxVector2 fbxUV(0, 0);
			bool isMapped;

			int vertexIndex = 0;

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);

				assert(iNumVertices == 3);

				//Possibly need to reverse order of vertices for fbx to direct X, but this results in empty spaces
				int controlIndex = pMesh->GetPolygonVertex(j, 0);
				pMesh->GetPolygonVertexUV(j, 0, "map1", fbxUV, isMapped);
				pMesh->GetPolygonVertexNormal(j, 0, fbxNorm);

				vertexArray[vertexIndex].point[0] = (float)pVertices[controlIndex].mData[0];
				vertexArray[vertexIndex].point[1] = (float)pVertices[controlIndex].mData[1];
				vertexArray[vertexIndex].point[2] = (float)pVertices[controlIndex].mData[2];

				vertexArray[vertexIndex].texCoord[0] = (float)fbxUV[0];
				vertexArray[vertexIndex].texCoord[1] = 1.0f - (float)fbxUV[1];

				vertexArray[vertexIndex].normal[0] = (float)fbxNorm[0];
				vertexArray[vertexIndex].normal[1] = (float)fbxNorm[1];
				vertexArray[vertexIndex].normal[2] = (float)fbxNorm[2];

				indexArray[vertexIndex] = vertexIndex;
				vertexIndex++;

				controlIndex = pMesh->GetPolygonVertex(j, 1);
				pMesh->GetPolygonVertexUV(j, 1, "map1", fbxUV, isMapped);
				pMesh->GetPolygonVertexNormal(j, 1, fbxNorm);

				vertexArray[vertexIndex].point[0] = (float)pVertices[controlIndex].mData[0];
				vertexArray[vertexIndex].point[1] = (float)pVertices[controlIndex].mData[1];
				vertexArray[vertexIndex].point[2] = (float)pVertices[controlIndex].mData[2];

				vertexArray[vertexIndex].texCoord[0] = (float)fbxUV[0];
				vertexArray[vertexIndex].texCoord[1] = 1.0f - (float)fbxUV[1];

				vertexArray[vertexIndex].normal[0] = (float)fbxNorm[0];
				vertexArray[vertexIndex].normal[1] = (float)fbxNorm[1];
				vertexArray[vertexIndex].normal[2] = (float)fbxNorm[2];

				indexArray[vertexIndex] = vertexIndex;
				vertexIndex++;

				controlIndex = pMesh->GetPolygonVertex(j, 2);
				pMesh->GetPolygonVertexUV(j, 2, "map1", fbxUV, isMapped);
				pMesh->GetPolygonVertexNormal(j, 2, fbxNorm);

				vertexArray[vertexIndex].point[0] = (float)pVertices[controlIndex].mData[0];
				vertexArray[vertexIndex].point[1] = (float)pVertices[controlIndex].mData[1];
				vertexArray[vertexIndex].point[2] = (float)pVertices[controlIndex].mData[2];

				vertexArray[vertexIndex].texCoord[0] = (float)fbxUV[0];
				vertexArray[vertexIndex].texCoord[1] = 1.0f - (float)fbxUV[1];

				vertexArray[vertexIndex].normal[0] = (float)fbxNorm[0];
				vertexArray[vertexIndex].normal[1] = (float)fbxNorm[1];
				vertexArray[vertexIndex].normal[2] = (float)fbxNorm[2];

				indexArray[vertexIndex] = vertexIndex;
				vertexIndex++;
			}



			*vBuf = renderer->createVertexBuffer(vertexArray, vertexCount);
			//*iBuf = renderer->createIndexBuffer(indices->data(), indices->size());
			*iBuf = renderer->createIndexBuffer(indexArray, numIndices);

			delete[] vertexArray;
			delete[] indexArray;
		}
	}
	return S_OK;
	}

}
