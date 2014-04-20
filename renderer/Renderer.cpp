#include "Renderer.h"
#include "OBJLoader.h"
#include "FBXLoader.h"


// Renderer Choice
#if RENDERER == RENDERER_DX
	#include "DirectX11\DX11Renderer.h"
#elif RENDERER == RENDERER_GL
	#include "OpenGL\GLRenderer.h"
#endif

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
	Creates a model from a file path. Returns pointer to model or nullptr if model failed to load or incorrect filepath.
		- filePath: path to 3D model file
		- vP: pointer to VertexBuffer pointer
		- iP: pointer to IndexBuffer pointer
*/
Model* Renderer::createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex) {
	char* extension;
	HRESULT result;

	extension = strrchr(filePath, '.');

	// Check extension and used necessary model loader
	if (strcmp(extension, ".obj") == 0){
		OBJLoader objLoaded;
		result = objLoaded.loadOBJFile(filePath, vP, iP, this);
	}
	else if (strcmp(extension, ".fbx") == 0){
		FBXLoader fileLoaded;
		result = fileLoaded.loadFBXFile(filePath, vP, iP, this);
	}
	else {
		return nullptr;
	}

	// Checks if error occured during model loader
	if (result == S_OK) {
		return createModel(*vP, *iP, tex);
	}
	else {
		return nullptr;
	}
}