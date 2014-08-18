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

		virtual ~Renderer();

		virtual void resize(unsigned int width, unsigned int height, bool fullscreen) = 0;

	private:
		template<typename T>
		class Factory {
		public:
			template<typename... Args>
			static T* create(Renderer* renderer, Args... args);
		};

	public:
		
		template<typename T, typename... Args>
		T* create(Args... args) {
			return Factory<T>::create(this, args...);
		}

		/* ----- */
		enum Dimension { TWO, THREE };

		virtual void clearFrame() = 0;
		virtual bool setLightBuffers(Common::Vector4* lightPositions, Common::Vector4* lightColors, int numLightsProvided) = 0;
		virtual void prep2D() = 0;
		virtual void end2D() = 0;

		virtual void makeTransparent() = 0;
		virtual void makeOpaque() = 0;
		virtual void turnDepthOff() = 0;
		virtual void turnDepthOn() = 0;
		virtual void useScreenCoords() = 0;
		virtual void drawFrame() = 0;

		virtual Shader* getDefaultVertexShader() = 0;
		virtual Shader* getDefaultPixelShader() = 0;

		virtual Camera* getCamera() = 0;

		virtual void setObjectMatrix(Matrix4) = 0;
	};
}
