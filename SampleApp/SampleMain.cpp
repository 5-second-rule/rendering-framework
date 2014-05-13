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

void moveBlob(Transmission::Window* w, Transmission::Model* m, Transmission::Camera* cam) {
	float moveAmt = 1000.0f / 1000.0f;
	Transmission::Input* input = (Transmission::Input*) w->getInput();

	if (input->getKeyState(Transmission::Input::Key::W) == Transmission::Input::STATE_DOWN) {
		m->move(Common::Vector4(0, moveAmt, 0));
		cam->move(Common::Vector4(0, moveAmt, 0));
		cam->lookAt(m->getPosition());
	}
	if (input->getKeyState(Transmission::Input::Key::A) == Transmission::Input::STATE_DOWN) {
		m->move(Common::Vector4(-moveAmt, 0, 0));
		cam->move(Common::Vector4(-moveAmt, 0, 0));
		cam->lookAt(m->getPosition());
	}
	if (input->getKeyState(Transmission::Input::Key::S) == Transmission::Input::STATE_DOWN) {
		m->move(Common::Vector4(0, -moveAmt, 0));
		cam->move(Common::Vector4(0, -moveAmt, 0));
		cam->lookAt(m->getPosition());
	}
	if (input->getKeyState(Transmission::Input::Key::D) == Transmission::Input::STATE_DOWN) {
		m->move(Common::Vector4(moveAmt, 0, 0));
		cam->move(Common::Vector4(moveAmt, 0, 0));
		cam->lookAt(m->getPosition());
	}
	if (input->getKeyState(Transmission::Input::Key::SPACE) == Transmission::Input::STATE_DOWN) {
		m->move(Common::Vector4(0, 0, moveAmt));
		cam->move(Common::Vector4(0, 0, moveAmt));
		cam->lookAt(m->getPosition());
	}

	if (input->getKeyState(Transmission::Input::Key::DOWN_ARROW) == Transmission::Input::STATE_DOWN) {
		m->move(Common::Vector4(0, 0, -moveAmt));
		cam->move(Common::Vector4(0, 0, -moveAmt));
		cam->lookAt(m->getPosition());
	}

	// THESE ROTATIONS ARE BROKEN, JUST DID SOMETHING TO TURN THE CAMERA A LITTLE
	if (input->getKeyState(Transmission::Input::Key::Q) == Transmission::Input::STATE_DOWN) {
		m->rotate(Common::Vector(0.00f, moveAmt, 0.0f));
		cam->lookAt(m->getPosition());
		cam->move(Common::Vector4(moveAmt, 0, moveAmt));
	}
	if (input->getKeyState(Transmission::Input::Key::E) == Transmission::Input::STATE_DOWN) {
		m->rotate(Common::Vector(0.00f, -moveAmt, 0.0f));
		cam->lookAt(m->getPosition());
		cam->move(Common::Vector4(-moveAmt, 0, -moveAmt));
	}

}

void changeShader(Transmission::Window* w, Transmission::Model* m, Transmission::Shader* defaultPixel, Transmission::Shader* otherPixel) {
	Transmission::Input* input = (Transmission::Input*) w->getInput();

	if (input->getKeyState(Transmission::Input::Key::O) == Transmission::Input::STATE_DOWN) {
		m->setPixelShader(defaultPixel);
	}

	if (input->getKeyState(Transmission::Input::Key::P) == Transmission::Input::STATE_DOWN) {
		m->setPixelShader(otherPixel);
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

	Transmission::VertexBuffer* tubeVbuf;
	Transmission::IndexBuffer* tubeIbuf;

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
	char* pipeTexture = "../SampleApp/bloodCell_TXTR.dds";

	Transmission::Shader* defaultVertexShad = renderer->createVertexShader("defaultVertex.cso");
	Transmission::Shader* vertRipple = renderer->createVertexShader("vertexRipple.cso");
	Transmission::Shader* vertSpring = renderer->createVertexShader("vertexSpring.cso");
	Transmission::Shader* vertWiggle = renderer->createVertexShader("vertexWiggle.cso");
	Transmission::Shader* defaultPixShad = renderer->createPixelShader("defaultPixel.cso");
	Transmission::Shader* pixShader = renderer->createPixelShader("pixel.cso");
	Transmission::Shader* pixShaderNoSpec = renderer->createPixelShader("pixelNoSpec.cso");
	Transmission::Shader* pixCelShader = renderer->createPixelShader("pixelCelShaded.cso");

	Transmission::Texture* herpesTex = renderer->createTextureFromFile(herpesTexture);
	Transmission::Texture* malariaTex = renderer->createTextureFromFile(malariaTexture);
	Transmission::Texture* poxTex = renderer->createTextureFromFile(poxTexture);
	Transmission::Texture* pipeTex = renderer->createTextureFromFile(pipeTexture);

	Transmission::Model* herpesModel = renderer->createModelFromFile(herpesFbxFilePath, &herpesVbuf, &herpesIbuf, herpesTex, true, vertRipple, pixShader);
	Transmission::Model* malariaModel = renderer->createModelFromFile(malariaFbxFilePath, &malariaVbuf, &malariaIbuf, malariaTex, true, vertWiggle, pixShader);
	Transmission::Model* poxModel = renderer->createModelFromFile(poxFbxFilePath, &poxVbuf, &poxIbuf, poxTex, true, vertSpring, pixShader);

	Transmission::Model* tubeModel = renderer->createModelFromFile("../SampleApp/track.obj", &tubeVbuf, &tubeIbuf, pipeTex, false);
	tubeModel->setPixelShader(pixShader);

	if (herpesModel == NULL) exit(-1);
	if (malariaModel == NULL) exit(-1);
	if (poxModel == NULL) exit(-1);
	if (tubeModel == NULL) exit(-1);

	herpesModel->move(Common::Vector4(0, 0, 35));
	malariaModel->move(Common::Vector4(-15, 0, 35));
	poxModel->move(Common::Vector4(15, 0, 35));
	poxModel->setScale(Common::Vector4(3.0, 3.0, 3.0, 0.0));

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

		poxModel->rotate(Common::Vector(0.00f, 200.0f*renderer->getTimer()->GetCalculatedTimeSinceLastFrame(), 0.0f));

		herpesModel->draw();
		malariaModel->draw();
		poxModel->draw();
		tubeModel->draw();

		renderer->drawFrame();

		odprintf("FPS: %d", fps);
		odprintf("Time: %f", renderer->getTimer()->GetTime());

		moveBlob(window, herpesModel, renderer->getCamera()); // temp input handler
		//moveBlob(window, malariaModel); // temp input handler
		//moveBlob(window, poxModel); // temp input handler
		//moveBlob(window, tubeModel);
		changeShader(window, tubeModel, defaultPixShad, pixShader);
	}

	delete herpesModel;
	delete malariaModel;
	delete poxModel;
	delete tubeModel;

	delete defaultVertexShad;
	delete vertRipple;
	delete vertSpring;
	delete vertWiggle;
	delete defaultPixShad;
	delete pixShader;
	delete pixShaderNoSpec;

	delete herpesVbuf;
	delete herpesIbuf;
	delete malariaVbuf;
	delete malariaIbuf;
	delete poxVbuf;
	delete poxIbuf;
	delete tubeVbuf;
	delete tubeIbuf;

	delete herpesTex;
	delete malariaTex;
	delete poxTex;
	delete pipeTex;

	delete renderer;
	delete window;
}