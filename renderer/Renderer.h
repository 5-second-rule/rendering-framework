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

namespace Transmission {


	class FRAMEWORKDLL Renderer
	{

	public:
		static Renderer* createRenderer( Window* window );
		virtual ~Renderer() = default;

		template<typename T, typename... Args>
		T* create(Args... args) {
			return Factory<T>::create(this, args...);
		}

		/* ----- */

		virtual void resize(unsigned int width, unsigned int height, bool fullscreen) = 0;
		virtual void clearFrame() = 0;

		virtual void turnDepthOff() = 0;
		virtual void turnDepthOn() = 0;

		virtual void drawFrame() = 0;

		virtual Camera* getCamera() = 0;

		virtual void setObjectMatrix(Matrix4) = 0;

	private:
		template<typename T>
		class Factory {
		public:
			template<typename... Args>
			static T* create(Renderer* renderer, Args... args);
		};
	};
}
