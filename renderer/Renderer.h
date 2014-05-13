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
		static Renderer* createRenderer(Window*);

		/* ----- */

		virtual void clearFrame() = 0;
		virtual void drawFrame() = 0;

		virtual VertexBuffer* createVertexBuffer(Vertex [], size_t) = 0;
		virtual IndexBuffer* createIndexBuffer(unsigned int [], size_t) = 0;
		virtual Model* createModel(VertexBuffer*, IndexBuffer*, Texture*) = 0;
		virtual Model* createModel(VertexBuffer*, IndexBuffer*, Texture*, Shader*, Shader*) = 0;

		virtual bool loadModelFile(char*, VertexBuffer**, IndexBuffer**, bool);
		virtual Model* createModelFromFile(char*, VertexBuffer**, IndexBuffer**, Texture*, bool);
		virtual Model* createModelFromFile(char*, VertexBuffer**, IndexBuffer**, Texture*, bool, Shader*, Shader*);
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
