#pragma once
#include "render-framework.h"
#include "util.h"

#include "Window.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "Timer.h"

namespace Transmission {


	class FRAMEWORKDLL Renderer
	{
	public:
		static Renderer* createRenderer( Window* window, char* vertex, char* pixel );

		/* ----- */
		enum Dimension { TWO, THREE };

		virtual void clearFrame() = 0;
		virtual void prep2D() = 0;
		virtual void end2D() = 0;
		virtual void makeTransparent() = 0;
		virtual void makeOpaque() = 0;
		virtual void turnDepthOff() = 0;
		virtual void turnDepthOn() = 0;
		virtual void useScreenCoords() = 0;
		virtual void drawFrame() = 0;

		virtual void turnDepthTestOff() = 0;
		virtual void turnDepthTestOn() = 0;

		virtual VertexBuffer* createVertexBuffer(Vertex [], size_t) = 0;
		virtual IndexBuffer* createIndexBuffer(unsigned int [], size_t) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, bool is2D) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps, bool is2D) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump, Shader* vs, Shader* ps) = 0;

		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture) = 0;
		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, Shader* vs, Shader* ps) = 0;
		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, bool isTransparent) = 0;
		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, Shader* vs, Shader* ps, bool isTransparent) = 0;

		virtual bool loadModelFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, bool centerShift);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift, bool is2D);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, Texture* bump, bool centerShift);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift, Shader* vs, Shader* ps);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift, Shader* vs, Shader* ps, bool is2D);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, Texture* bump, bool centerShift, Shader* vs, Shader* ps);
				
		virtual Texture* createTextureFromFile(char*) = 0;
		virtual Shader* createVertexShader(char*) = 0;
		virtual Shader* createPixelShader(char*) = 0;

		virtual Shader* getDefaultVertexShader() = 0;
		virtual Shader* getDefaultPixelShader() = 0;

		virtual Camera* getCamera() = 0;

		virtual Timer* getTimer() = 0;

		virtual void setObjectMatrix(Matrix4) = 0;
	};
}
