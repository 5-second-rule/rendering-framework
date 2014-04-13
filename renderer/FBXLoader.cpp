#include "FBXLoader.h"
#include "fbxsdk.h"

#include <Windows.h>
#pragma comment(lib, "libfbxsdk.lib")

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

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		// Check if the getChildCount is > 1  TODO
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
			int controlCount = pMesh->GetControlPointsCount();

			Vertex vertex;
			Vertex* vertexArray = new Vertex[controlCount];

			for (int l = 0; l < controlCount; l++)
			{
				//The divide by 2.01 and the add by 1.1 is accounting for the fact that the model has z values ranging from ~-1.07 to 1.07 
				//when it needs to range from 0 to 1
				vertex.point = XMFLOAT3((float)pVertices[l].mData[0] / 2.01f, 
										(float)pVertices[l].mData[1] / 2.01f, 
										((float)pVertices[l].mData[2] + 1.1f) / 2.01f);
				vertex.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
				vertexArray[l] = vertex;
			}

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);
				//OutputDebugString(L"above\n");
				assert(iNumVertices == 3);

				//Possibly need to reverse order of vertices for fbx to direct X, but this results in empty spaces
				//indices->push_back(pMesh->GetPolygonVertex(j, 0));
				//indices->push_back(pMesh->GetPolygonVertex(j, 2));
				//indices->push_back(pMesh->GetPolygonVertex(j, 1));

				for (int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					indices->push_back(iControlPointIndex);
				}
			}

			*vBuf = renderer->createVertexBuffer(vertexArray, controlCount);
			*iBuf = renderer->createIndexBuffer(indices->data(), indices->size());

			delete vertexArray;
		}
	}
	return S_OK;
}
