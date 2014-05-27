#include "Renderer.h"
#include "OBJLoader.h"
#include "FBXLoader.h"
#include "TRKLoader.h"

// Renderer Choice
#if RENDERER == RENDERER_DX
	#include "DirectX11\DX11Renderer.h"
#elif RENDERER == RENDERER_GL
	#include "OpenGL\GLRenderer.h"
#endif

namespace Transmission {

	Renderer* Renderer::createRenderer(Window* window, char* vertex, char* pixel) {

#if RENDERER == RENDERER_DX
		return new DX11Renderer(window, vertex, pixel);
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
		else if (strcmp(extension, ".trk") == 0){
			TRKLoader fileLoaded;
			result = fileLoaded.loadTRKFile(filePath, vP, iP, this);
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
	- tex: pointer to texture
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
	Creates a model from a file path. Returns pointer to model or nullptr if model failed to load or incorrect filepath.
	- filePath: path to 3D model file
	- vP: pointer to VertexBuffer pointer
	- iP: pointer to IndexBuffer pointer
	- tex: pointer to texture
	*/
	Model* Renderer::createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift, bool is2D) {
		// Checks if error occured during model load
		if (this->loadModelFile(filePath, vP, iP, centerShift)) {
			return createModel(*vP, *iP, tex, is2D);
		}
		else {
			return nullptr;
		}
	}

	/*
	Creates a model from a file path. Returns pointer to model or nullptr if model failed to load or incorrect filepath.
	- filePath: path to 3D model file
	- vP: pointer to VertexBuffer pointer
	- iP: pointer to IndexBuffer pointer
	- tex: pointer to texture
	- bump: pointer to bumpMap texture
	*/
	Model* Renderer::createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, Texture* bump, bool centerShift) {
		// Checks if error occured during model load
		if (this->loadModelFile(filePath, vP, iP, centerShift)) {
			return createModel(*vP, *iP, tex, bump);
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
	- tex: pointer to texture
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

	/*
	Creates a model from a file path that uses set shaders. Returns pointer to model or nullptr if model failed to load or incorrect filepath.
	- filePath: path to 3D model file
	- vP: pointer to VertexBuffer pointer
	- iP: pointer to IndexBuffer pointer
	- tex: pointer to texture
	- vertexShader : pointer to vertex shader
	- pixelShader : pointer to pixel shader
	*/
	Model* Renderer::createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift, Shader* vertexShader, Shader* pixelShader, bool is2D) {
		// Checks if error occured during model load
		if (this->loadModelFile(filePath, vP, iP, centerShift)) {
			return createModel(*vP, *iP, tex, vertexShader, pixelShader, is2D);
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
	- tex: pointer to texture
	- bump: pointer to bumpMap texture
	- vertexShader : pointer to vertex shader
	- pixelShader : pointer to pixel shader
	*/
	Model* Renderer::createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, Texture* bump, bool centerShift, Shader* vertexShader, Shader* pixelShader) {
		// Checks if error occured during model load
		if (this->loadModelFile(filePath, vP, iP, centerShift)) {
			return createModel(*vP, *iP, tex, bump, vertexShader, pixelShader);
		}
		else {
			return nullptr;
		}
	}
}