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

struct lightData
{
	float position[4];
	float color[4];
};

namespace Transmission {


	class FRAMEWORKDLL Renderer
	{
	public:
		static Renderer* createRenderer( Window* window, char* vertex, char* pixel );

		virtual void resize(unsigned int width, unsigned int height, bool fullscreen) = 0;

		/* ----- */

		virtual void clearFrame() = 0;
		virtual bool setLightBuffers(Common::Vector4* lightPositions, int numPositions, Common::Vector4* lightColors, int numColors) = 0;
		virtual void makeTransparent() = 0;
		virtual void makeOpaque() = 0;
		virtual void drawFrame() = 0;

		virtual void turnDepthTestOff() = 0;
		virtual void turnDepthTestOn() = 0;

		virtual VertexBuffer* createVertexBuffer(Vertex [], size_t) = 0;
		virtual IndexBuffer* createIndexBuffer(unsigned int [], size_t) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps) = 0;
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump, Shader* vs, Shader* ps) = 0;

		virtual bool loadModelFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, bool centerShift);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, Texture* bump, bool centerShift);
		virtual Model* createModelFromFile(char* filePath, VertexBuffer** vP, IndexBuffer** iP, Texture* tex, bool centerShift, Shader* vs, Shader* ps);
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
