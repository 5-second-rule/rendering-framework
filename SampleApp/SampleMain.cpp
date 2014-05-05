#include <windows.h>

#include "renderer/Renderer.h"
#include "renderer/Window.h"

#include "renderer/FBXLoader.h"

#include "renderer/Input.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool messagePump(Transmission::Window* window) {
	Transmission::Window::MessageType t;
	while ((t = window->getMessage()) != Transmission::Window::MessageType::None) {
		if (t == Transmission::Window::MessageType::Quit) return false;
	}

	return true;
}

void moveBlob(Transmission::Window* w, Transmission::Model* m) {
	float moveAmt = 100.0f / 1000.0f;
	Transmission::Input* input = (Transmission::Input*) w->getInput();

	if (input->getKeyState(Transmission::Input::Key::W) == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(0, moveAmt, 0));
	}
	if (input->getKeyState(Transmission::Input::Key::A) == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(-moveAmt, 0, 0));
	}
	if (input->getKeyState(Transmission::Input::Key::S) == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(0, -moveAmt, 0));
	}
	if (input->getKeyState(Transmission::Input::Key::D) == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(moveAmt, 0, 0));
	}
	if (input->getKeyState(Transmission::Input::Key::SPACE) == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(0, 0, moveAmt));
	}
	if (input->getKeyState(Transmission::Input::Key::Q) == Transmission::Input::STATE_DOWN) {
		m->rotate(Transmission::Vector(0.00f, moveAmt, 0.0f));
	}
	if (input->getKeyState(Transmission::Input::Key::E) == Transmission::Input::STATE_DOWN) {
		m->rotate(Transmission::Vector(0.00f, -moveAmt, 0.0f));
	}

}

// Temporary formatted output function for writing to the output window
void __cdecl odprintf(const char *format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = _vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	wchar_t wtext[4100];
	mbstowcs(wtext, buf, strlen(buf) + 1);//Plus null
	LPWSTR ptr = wtext;

	OutputDebugString(ptr);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Transmission::Window* window = Transmission::Window::createWindow(hInstance);
	Transmission::Renderer* renderer = Transmission::Renderer::createRenderer(window);

	Transmission::VertexBuffer* herpesVbuf;
	Transmission::IndexBuffer* herpesIbuf;

	Transmission::VertexBuffer* malariaVbuf;
	Transmission::IndexBuffer* malariaIbuf;

	Transmission::VertexBuffer* poxVbuf;
	Transmission::IndexBuffer* poxIbuf;

	char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
	char* ecoliFbxFilePath = "../SampleApp/ecoli6_nomedia.fbx";
	char* ecoliObjFilePath = "../SampleApp/Ecoli6_obj.obj";
	char* boxFbxFilePath = "../SampleApp/cube.fbx";
	char* herpesFbxFilePath = "../SampleApp/herpes2.fbx";
	char* malariaFbxFilePath = "../SampleApp/malaria.fbx";
	char* poxFbxFilePath = "../SampleApp/pox.fbx";

	char* textureLocation = "../SampleApp/ecoli6_TXTR.dds";
	char* textureLocationW = "../SampleApp/Wood.dds";
	char* whiteTexture = "../SampleApp/whitebloodcell_3_TXTR.dds";
	char* cubeTexture = "../SampleApp/cube_uvmap2.dds";
	char* herpesTexture = "../SampleApp/herpes_3_TXTR.dds";
	char* malariaTexture = "../SampleApp/malaria_TXTR.dds";
	char* poxTexture = "../SampleApp/pox_TXTR.dds";

	Transmission::Shader* vertRipple = renderer->createVertexShader("vertexRipple.cso");
	Transmission::Shader* vertSpring = renderer->createVertexShader("vertexSpring.cso");
	Transmission::Shader* vertWiggle = renderer->createVertexShader("vertexWiggle.cso");
	Transmission::Shader* pixShader = renderer->createPixelShader("pixel.cso");
	Transmission::Shader* pixShaderNoSpec = renderer->createPixelShader("pixelNoSpec.cso");
	Transmission::Shader* pixCelShader = renderer->createPixelShader("pixelCelShaded.cso");

	Transmission::Texture* herpesTex = renderer->createTextureFromFile(herpesTexture);
	Transmission::Texture* malariaTex = renderer->createTextureFromFile(malariaTexture);
	Transmission::Texture* poxTex = renderer->createTextureFromFile(poxTexture);

	Transmission::Model* herpesModel = renderer->createModelFromFile(herpesFbxFilePath, &herpesVbuf, &herpesIbuf, herpesTex, vertRipple, pixShader);
	Transmission::Model* malariaModel = renderer->createModelFromFile(malariaFbxFilePath, &malariaVbuf, &malariaIbuf, malariaTex, vertWiggle, pixShader);
	Transmission::Model* poxModel = renderer->createModelFromFile(poxFbxFilePath, &poxVbuf, &poxIbuf, poxTex, vertSpring, pixShader);

	if (herpesModel == NULL) exit(-1);
	if (malariaModel == NULL) exit(-1);
	if (poxModel == NULL) exit(-1);

	herpesModel->move(Transmission::Vector4(0, 0, 35));
	malariaModel->move(Transmission::Vector4(-15, 0, 35));
	poxModel->move(Transmission::Vector4(15, 0, 35));
	poxModel->setScale(Transmission::Vector4(3.0, 3.0, 3.0, 0.0));

	renderer->getTimer()->StartTimer();

	int frameCount = 0;
	int fps = 0;

	while (messagePump(window)) {
		renderer->clearFrame();

		frameCount++;
		if (renderer->getTimer()->GetFPSTime() > 1.0f)
		{
			fps = frameCount;
			frameCount = 0;
			renderer->getTimer()->ResetFPSTimer();
		}

		poxModel->rotate(Transmission::Vector(0.00f, 200.0f*renderer->getTimer()->GetCalculatedTimeSinceLastFrame(), 0.0f));

		herpesModel->draw();
		malariaModel->draw();
		poxModel->draw();

		renderer->drawFrame();

		odprintf("FPS: %d", fps);
		odprintf("Time: %f", renderer->getTimer()->GetTime());

		moveBlob(window, herpesModel); // temp input handler
		moveBlob(window, malariaModel); // temp input handler
		moveBlob(window, poxModel); // temp input handler
	}

	delete herpesModel;
	delete malariaModel;
	delete poxModel;

	delete vertRipple;
	delete vertSpring;
	delete vertWiggle;
	delete pixShader;
	delete pixShaderNoSpec;

	delete herpesVbuf;
	delete herpesIbuf;
	delete malariaVbuf;
	delete malariaIbuf;
	delete poxVbuf;
	delete poxIbuf;

	delete herpesTex;
	delete malariaTex;
	delete poxTex;

	delete renderer;
	delete window;
}