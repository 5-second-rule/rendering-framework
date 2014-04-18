#include "OBJLoader.h"


OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

////////////////////////////////////////////////////////////////////////////////
// Filename: OBJLoader.cpp
////////////////////////////////////////////////////////////////////////////////


//////////////
// INCLUDES //
//////////////
//#include <iostream>
#include <fstream>
using namespace std;


//////////////
// TYPEDEFS //
//////////////

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;

//////////////////
// MAIN PROGRAM //
//////////////////
HRESULT OBJLoader::loadOBJFile(char* filePath, VertexBuffer** vBuf, IndexBuffer** iBuf, Renderer* renderer) {
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;

	// Read in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	result = ReadFileCounts(filePath, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	// Now read the data from the file into the data structures and then output it in our model format.
	result = LoadDataStructures(filePath, vertexCount, textureCount, normalCount, faceCount, vBuf, iBuf, renderer);
	if (!result)
	{
		return -1;
	}

	return 0;
}

bool OBJLoader::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}


bool OBJLoader::LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, VertexBuffer** vBuf, IndexBuffer** iBuf, Renderer* renderer)
{
	Vertex *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex;
	char input, input2;


	// Initialize the four data structures.
	vertices = new Vertex[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new Vertex[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new Vertex[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].point[0] >> vertices[vertexIndex].point[1] >> vertices[vertexIndex].point[2];

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].point[2] = vertices[vertexIndex].point[2] * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].point[0] >> texcoords[texcoordIndex].point[1];

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].point[1] = 1.0f - texcoords[texcoordIndex].point[1];
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].point[0] >> normals[normalIndex].point[1] >> normals[normalIndex].point[2];

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].point[2] = normals[normalIndex].point[2] * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();


	int numIndices = faceCount * 3;
	unsigned int* indices = new unsigned int[numIndices];
	int indiceIndex = 0;
	// Now loop through all the faces and create the index buffer
	for (int i = 0; i<faceIndex; i++)
	{
		/*
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].point.x << ' ' << vertices[vIndex].point.y << ' ' << vertices[vIndex].point.z << ' '
			<< texcoords[tIndex].point.x << ' ' << texcoords[tIndex].point.y << ' '
			<< normals[nIndex].point.x << ' ' << normals[nIndex].point.y << ' ' << normals[nIndex].point.z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].point.x << ' ' << vertices[vIndex].point.y << ' ' << vertices[vIndex].point.z << ' '
			<< texcoords[tIndex].point.x << ' ' << texcoords[tIndex].point.y << ' '
			<< normals[nIndex].point.x << ' ' << normals[nIndex].point.y << ' ' << normals[nIndex].point.z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].point.x << ' ' << vertices[vIndex].point.y << ' ' << vertices[vIndex].point.z << ' '
			<< texcoords[tIndex].point.x << ' ' << texcoords[tIndex].point.y << ' '
			<< normals[nIndex].point.x << ' ' << normals[nIndex].point.y << ' ' << normals[nIndex].point.z << endl;
		*/

		indices[indiceIndex++] = faces[i].vIndex1;
		indices[indiceIndex++] = faces[i].vIndex2;
		indices[indiceIndex++] = faces[i].vIndex3;
	}

	*vBuf = renderer->createVertexBuffer(vertices, vertexCount);
	*iBuf = renderer->createIndexBuffer(indices, numIndices);

	delete[] indices;
	indices = 0;

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}