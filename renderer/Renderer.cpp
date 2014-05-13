#include "Renderer.h"
#include "OBJLoader.h"
#include "FBXLoader.h"

// Renderer Choice
#if RENDERER == RENDERER_DX
	#include "DirectX11\DX11Renderer.h"
#elif RENDERER == RENDERER_GL
	#include "OpenGL\GLRenderer.h"
#endif

namespace Transmission {

	Renderer* Renderer::createRenderer(Window* window) {

#if RENDERER == RENDERER_DX
		return new DX11Renderer(window);
#elif RENDERER == RENDERER_GL
		return new GLRenderer(window);
#else
#error "Unsupported Renderer"
#endif
	}

	/*
	Load a model from a file path. Returns false if model failed to load or incorrect filepath.
	- filePath: path to 3D model file
	- vP: pointer to VertexBuffer pointer
	- iP: pointer to IndexBuffer pointer
	*/
	bool Renderer::loadModelFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, bool centerShift) {
		char* extension;
		HRESULT result;

		extension = strrchr(filePath, '.');

		// Check extension and used necessary model loader
		if (strcmp(extension, ".obj") == 0){
			OBJLoader objLoaded;
			result = objLoaded.loadOBJFile(filePath, vP, iP, this, centerShift); //TODO handle selecting center shift for some objects
		}
		else if (strcmp(extension, ".fbx") == 0){
			FBXLoader fileLoaded;
			result = fileLoaded.loadFBXFile(filePath, vP, iP, this, centerShift);
		}
		else {
			return false;
		}

		return result == S_OK;
	}

	/*
	Creates a model from a file path. Returns pointer to model or nullptr if model failed to load or incorrect filepath.
	- filePath: path to 3D model file
	- vP: pointer to VertexBuffer pointer
	- iP: pointer to IndexBuffer pointer
	*/
	Model* Renderer::createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift) {
		// Checks if error occured during model load
		if (this->loadModelFile(filePath, vP, iP, centerShift)) {
			return createModel(*vP, *iP, tex);
		}
		else {
			return nullptr;
		}
	}

	/*
	Creates a model from a file path that uses set shaders. Returns pointer to model or nullptr if model failed to load or incorrect filepath.
	- filePath: path to 3D model file
	- vP: pointer to VertexBuffer pointer
	- iP: pointer to IndexBuffer pointer
	- vertexShader : pointer to vertex shader
	- pixelShader : pointer to pixel shader
	*/
	Model* Renderer::createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift, Shader* vertexShader, Shader* pixelShader) {
		// Checks if error occured during model load
		if (this->loadModelFile(filePath, vP, iP, centerShift)) {
			return createModel(*vP, *iP, tex, vertexShader, pixelShader);
		}
		else {
			return nullptr;
		}
	}
}