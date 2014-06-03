#include <windows.h>

#include "renderer/Renderer.h"
#include "renderer/Window.h"

#include "renderer/FBXLoader.h"

#include "renderer/Input.h"

static bool renderSelection = false;
static int currModel = 0;
static int frameCount;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool messagePump(Transmission::Window* window) {
	Transmission::Window::MessageType t;
	while ((t = window->getMessage()) != Transmission::Window::MessageType::None) {
		if (t == Transmission::Window::MessageType::Quit) return false;
	}

	return true;
}

void moveBlob(Transmission::Window* w, Transmission::Model* m, Transmission::Camera* cam) {
	float moveAmt = 10000.0f / 1000.0f;
	Transmission::Input* input = (Transmission::Input*) w->getInput();

	if (input->getKeyState(Transmission::Input::Key::KEY_1) == Transmission::Input::STATE_DOWN) {
		renderSelection = true;
	}

	if (input->getKeyState(Transmission::Input::Key::KEY_2) == Transmission::Input::STATE_DOWN) {
		renderSelection = false;
	}

	if (!renderSelection) {
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
		if (input->getKeyState(Transmission::Input::Key::I) == Transmission::Input::STATE_DOWN) {
			m->move(Common::Vector4(0, 0, moveAmt));
			cam->move(Common::Vector4(0, 0, moveAmt));
			cam->lookAt(m->getPosition());
		}

		if (input->getKeyState(Transmission::Input::Key::O) == Transmission::Input::STATE_DOWN) {
			m->move(Common::Vector4(0, 0, -moveAmt));
			cam->move(Common::Vector4(0, 0, -moveAmt));
			cam->lookAt(m->getPosition());
		}

		//Rotate Up
		if (input->getKeyState(Transmission::Input::Key::UP_ARROW) == Transmission::Input::STATE_DOWN) {
			m->rotate(moveAmt / 20.0f, 0, 0);
			cam->lookAt(m->getPosition());
		}

		//Rotate Down
		if (input->getKeyState(Transmission::Input::Key::DOWN_ARROW) == Transmission::Input::STATE_DOWN) {
			m->rotate(-moveAmt / 20.0f, 0, 0);
			cam->lookAt(m->getPosition());
		}

		//Rotate Left
		if (input->getKeyState(Transmission::Input::Key::LEFT_ARROW) == Transmission::Input::STATE_DOWN) {
			m->rotate(0, moveAmt / 20.0f, 0);
			cam->lookAt(m->getPosition());
		}

		//Rotate Right
		if (input->getKeyState(Transmission::Input::Key::RIGHT_ARROW) == Transmission::Input::STATE_DOWN) {
			m->rotate(0, -moveAmt / 20.0f, 0);
			cam->lookAt(m->getPosition());
		}

		//Zoom in
		if (input->getKeyState(Transmission::Input::Key::Z) == Transmission::Input::STATE_DOWN) {
			Common::Vector4 direction;
			direction = (m->getPosition() - cam->getPosition());
			direction.normalize();
			cam->move(direction*moveAmt);
			cam->lookAt(m->getPosition());
		}

		//Zoom out
		if (input->getKeyState(Transmission::Input::Key::X) == Transmission::Input::STATE_DOWN) {
			Common::Vector4 direction;
			direction = (cam->getPosition() - m->getPosition());
			direction.normalize();
			cam->move(direction*moveAmt);
			cam->lookAt(m->getPosition());
		}
		// THESE ROTATIONS ARE BROKEN, JUST DID SOMETHING TO TURN THE CAMERA A LITTLE
		if (input->getKeyState(Transmission::Input::Key::Q) == Transmission::Input::STATE_DOWN) {
			cam->lookAt(m->getPosition());
			cam->move(Common::Vector4(moveAmt, 0, moveAmt));
		}
		if (input->getKeyState(Transmission::Input::Key::E) == Transmission::Input::STATE_DOWN) {
			cam->lookAt(m->getPosition());
			cam->move(Common::Vector4(-moveAmt, 0, -moveAmt));
		}
	} else if (frameCount % 100 == 0) {
		if (input->getKeyState(Transmission::Input::Key::LEFT_ARROW) == Transmission::Input::STATE_DOWN) {
			currModel = (currModel + 3) % 4;
		}
		if (input->getKeyState(Transmission::Input::Key::RIGHT_ARROW) == Transmission::Input::STATE_DOWN) {
			currModel = (currModel + 1) % 4;
		}
	}
}

void changeShader(Transmission::Window* w, Transmission::Model* m, Transmission::Shader* defaultPixel, Transmission::Shader* otherPixel) {
	Transmission::Input* input = (Transmission::Input*) w->getInput();

	if (input->getKeyState(Transmission::Input::Key::K) == Transmission::Input::STATE_DOWN) {
		m->setPixelShader(defaultPixel);
	}

	if (input->getKeyState(Transmission::Input::Key::L) == Transmission::Input::STATE_DOWN) {
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

void title(Transmission::Vertex(&vertices)[4], float winHeight, float winWidth) {
	float scale;
	if (winHeight < winWidth)
		scale = winHeight / winWidth / 2;
	else 
		scale = winWidth / winHeight / 2;

	float l = -2.0 * scale,
		r = 2.0 * scale,
		t = 1.0,
		b = 1.0 - scale;

	vertices[0] = { { l, t, 0.0f }, { 0, 0 }, { 0, 0, -1 }, {} };
	vertices[1] = { { r, t, 0.0f }, { 1, 0 }, { 0, 0, -1 }, {} };
	vertices[2] = { { r, b, 0.0f }, { 1, 1 }, { 0, 0, -1 }, {} };
	vertices[3] = { { l, b, 0.0f }, { 0, 1 }, { 0, 0, -1 }, {} };
}

void selectionBG(Transmission::Vertex (&vertices)[4]) {
	vertices[0] = { { -1.0f,  1.0f, 0.0f }, { 0, 0 }, { 0, 0, -1 }, {} };
	vertices[1] = { {  1.0f,  1.0f, 0.0f }, { 1, 0 }, { 0, 0, -1 }, {} };
	vertices[2] = { {  1.0f, -1.0f, 0.0f }, { 1, 1 }, { 0, 0, -1 }, {} };
	vertices[3] = { { -1.0f, -1.0f, 0.0f }, { 0, 1 }, { 0, 0, -1 }, {} };
}

float player(Transmission::Vertex (&vertices)[4], int playerIndex, float margin) {
	float numMargins[] = { -1.5, -0.5, 0.5, 1.5 };
	float pos[] = { -2, -1, 0, 1 };

	float edgeT = 0.5;
	float edgeB = -0.7;

	float width = (2.0 - 5 * margin) / 4;

	float edgeL = pos[playerIndex] * width + numMargins[playerIndex] * margin;
	float edgeR = edgeL + width;

	vertices[0] = { { edgeL, edgeT, 0.0f }, { 0, 0 }, { 0, 0, -1 }, {} };
	vertices[1] = { { edgeR, edgeT, 0.0f }, { 1, 0 }, { 0, 0, -1 }, {} };
	vertices[2] = { { edgeR, edgeB, 0.0f }, { 1, 1 }, { 0, 0, -1 }, {} };
	vertices[3] = { { edgeL, edgeB, 0.0f }, { 0, 1 }, { 0, 0, -1 }, {} };

	return edgeL + width / 2;
}

float ui(Transmission::Vertex (&vertices)[4], int playerIndex) {
	float numMargins[] = { 1.5, 0.5, -0.5, -1.5 };
	float pos[] = { 1, 0, -1, -2 };

	float margin = 0.08;
	float edgeMargin = 0.4;
	
	float width = 0.15;

	float edgeL = -1.0 + 0.1;
	float edgeR = edgeL + width;
	
	float edgeB = pos[playerIndex] * width + numMargins[playerIndex] * margin;
	float edgeT = edgeB + width;

	vertices[0] = { { edgeL, edgeT, 0.0f }, { 0, 0 }, { 0, 0, -1 }, {} };
	vertices[1] = { { edgeR, edgeT, 0.0f }, { 1, 0 }, { 0, 0, -1 }, {} };
	vertices[2] = { { edgeR, edgeB, 0.0f }, { 1, 1 }, { 0, 0, -1 }, {} };
	vertices[3] = { { edgeL, edgeB, 0.0f }, { 0, 1 }, { 0, 0, -1 }, {} };

	return edgeL + width / 2;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Transmission::Window* window = Transmission::Window::createWindow(hInstance, L"5 Second Rule", 800, 600);
	Transmission::Renderer* renderer = Transmission::Renderer::createRenderer(window, "defaultVertex.cso", "defaultPixel.cso");

	Transmission::VertexBuffer* ecoliVbuf;
	Transmission::IndexBuffer* ecoliIbuf;

	Transmission::VertexBuffer* herpesVbuf;
	Transmission::IndexBuffer* herpesIbuf;

	Transmission::VertexBuffer* malariaVbuf;
	Transmission::IndexBuffer* malariaIbuf;

	Transmission::VertexBuffer* poxVbuf;
	Transmission::IndexBuffer* poxIbuf;

	Transmission::VertexBuffer* tubeVbuf;
	Transmission::IndexBuffer* tubeIbuf;

	char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
	char* ecoliFbxFilePath = "../SampleApp/ecoliii.fbx";
	char* boxFbxFilePath = "../SampleApp/cube.fbx";
	char* herpesFbxFilePath = "../SampleApp/herpes2.fbx";
	char* malariaFbxFilePath = "../SampleApp/malaria.fbx";
	char* poxFbxFilePath = "../SampleApp/pox.fbx";

	char* trackFilePath = "../SampleApp/track.trk";

	char* ecoliTexture = "../SampleApp/ecolizzz_TXTR.dds";
	char* whiteTexture = "../SampleApp/whitebloodcell_3_TXTR.dds";
	char* cubeTexture = "../SampleApp/cube_uvmap2.dds";
	char* herpesTexture = "../SampleApp/herpes_3_TXTR.dds";
	char* malariaTexture = "../SampleApp/malaria_TXTR.dds";
	char* poxTexture = "../SampleApp/pox_TXTR.dds";
	char* pipeTexture = "../SampleApp/bloodCell_TXTR.dds";

	char* pipeBump = "../SampleApp/bloodCellBump.dds";

	char* titleTexture = "../SampleApp/select-title.dds";
	char* windowBgTexture = "../SampleApp/select-background.dds";
	char* playerBgTexture = "../SampleApp/select-rectangle.dds";
	char* uiTexture = "../SampleApp/pink.dds";

	Transmission::Shader* defaultVertexShad = renderer->createVertexShader("defaultVertex.cso");
	Transmission::Shader* vertRipple = renderer->createVertexShader("vertexRipple.cso");
	Transmission::Shader* vertSpring = renderer->createVertexShader("vertexSpring.cso");
	Transmission::Shader* vertTent = renderer->createVertexShader("vertexTentacle.cso");
	Transmission::Shader* vertTrack = renderer->createVertexShader("vertexTrack.cso");
	Transmission::Shader* vertWiggle = renderer->createVertexShader("vertexWiggle.cso");

	Transmission::Shader* defaultPixShad = renderer->createPixelShader("defaultPixel.cso");
	Transmission::Shader* pixShader = renderer->createPixelShader("pixel.cso");
	Transmission::Shader* pixBump = renderer->createPixelShader("pixelBump.cso");
	Transmission::Shader* pixShaderNoSpec = renderer->createPixelShader("pixelNoSpec.cso");
	Transmission::Shader* pixCelShader = renderer->createPixelShader("pixelCelShaded.cso");

	Transmission::Texture* ecoliTex = renderer->createTextureFromFile(ecoliTexture);
	Transmission::Texture* herpesTex = renderer->createTextureFromFile(herpesTexture);
	Transmission::Texture* malariaTex = renderer->createTextureFromFile(malariaTexture);
	Transmission::Texture* poxTex = renderer->createTextureFromFile(poxTexture);
	Transmission::Texture* pipeTex = renderer->createTextureFromFile(pipeTexture);
	Transmission::Texture* pipeBumpTex = renderer->createTextureFromFile(pipeBump);

	Transmission::Texture* titleTex = renderer->createTextureFromFile(titleTexture);
	Transmission::Texture* windowBgTex = renderer->createTextureFromFile(windowBgTexture);
	Transmission::Texture* playerBgTex = renderer->createTextureFromFile(playerBgTexture);
	Transmission::Texture* uiTex = renderer->createTextureFromFile(uiTexture);

	Transmission::Model* ecoliModel = renderer->createModelFromFile(ecoliFbxFilePath, &ecoliVbuf, &ecoliIbuf, ecoliTex, true, vertTent, pixShader);
	Transmission::Model* herpesModel = renderer->createModelFromFile(herpesFbxFilePath, &herpesVbuf, &herpesIbuf, herpesTex, true, vertRipple, pixShader);
	Transmission::Model* malariaModel = renderer->createModelFromFile(malariaFbxFilePath, &malariaVbuf, &malariaIbuf, malariaTex, true, vertWiggle, pixShader);
	Transmission::Model* poxModel = renderer->createModelFromFile(poxFbxFilePath, &poxVbuf, &poxIbuf, poxTex, true, vertSpring, pixShader);

	Transmission::Model* ecoliSelect = renderer->createModelFromFile(ecoliFbxFilePath, &ecoliVbuf, &ecoliIbuf, ecoliTex, true, vertTent, pixShader);
	Transmission::Model* herpesSelect = renderer->createModelFromFile(herpesFbxFilePath, &herpesVbuf, &herpesIbuf, herpesTex, true, vertRipple, pixShader);
	Transmission::Model* malariaSelect = renderer->createModelFromFile(malariaFbxFilePath, &malariaVbuf, &malariaIbuf, malariaTex, true, vertWiggle, pixShader);
	Transmission::Model* poxSelect = renderer->createModelFromFile(poxFbxFilePath, &poxVbuf, &poxIbuf, poxTex, true, vertSpring, pixShader);

	Transmission::Model* tubeModel = renderer->createModelFromFile(trackFilePath, &tubeVbuf, &tubeIbuf, pipeTex, pipeBumpTex, false, vertTrack, pixBump);


	Transmission::Index indices[] = { 0, 1, 2, 3, 0, 2 };
	Transmission::Vertex vertices[4];

	title(vertices, window->getHeight(), window->getWidth());
	Transmission::Model* titleModel = renderer->create2DModelFromVertices(vertices, 4, indices, 6, titleTex);

	selectionBG(vertices);
	Transmission::Model* windowBgModel = renderer->create2DModelFromVertices(vertices, 4, indices, 6, windowBgTex);

	float margin = 0.05;
	float width = (2.0 - 5 * margin) / 4;
	float centers[4];

	Transmission::Texture* playerTex[4];
	Transmission::Model* playerModels[4];
	Transmission::Model* playerBgModel[4];
	Transmission::Model* uiModels[4];

	for (int i = 0; i < 4; ++i) {
		centers[i] = player(vertices, i, margin) * 5.5;
		playerBgModel[i] = renderer->create2DModelFromVertices(vertices, 4, indices, 6, playerBgTex);

		ui(vertices, i);
		uiModels[i] = renderer->create2DModelFromVertices(vertices, 4, indices, 6, uiTex);
	}


	if (ecoliModel == NULL) exit(-1);
	if (herpesModel == NULL) exit(-1);
	if (malariaModel == NULL) exit(-1);
	if (poxModel == NULL) exit(-1);
	if (tubeModel == NULL) exit(-1);
	if (titleModel == NULL) exit(-1);
	if (windowBgModel == NULL) exit(-1);
	for (int i = 0; i < 4; ++i) {
		if (playerModels[i] == NULL) exit(-1);
		if (playerBgModel[i] == NULL) exit(-1);
	}

	renderer->getTimer()->StartTimer();

	//TODO handle size
	Common::Vector4 lightPositions[4];
	Common::Vector4 lightColors[4];

	frameCount = 0;

	int fps = 0;

	float scale = width / 2;

	herpesSelect->setScale(scale);
	herpesModel->setScale(scale);
	malariaSelect->setScale(scale);
	malariaModel->setScale(scale);
	scale *= 1.2;
	ecoliSelect->setScale(scale);
	ecoliModel->setScale(scale);
	scale = 0.8;
	poxSelect->setScale(scale);
	poxModel->setScale(scale);

	playerModels[0] = ecoliSelect;
	playerModels[1] = herpesSelect;
	playerModels[2] = malariaSelect;
	playerModels[3] = poxSelect;

	Transmission::Model* moreModels[4];
	moreModels[0] = ecoliModel;
	moreModels[1] = herpesModel;
	moreModels[2] = malariaModel;
	moreModels[3] = poxModel;

	bool newSelected = false;
	playerModels[currModel]->setPosition(Common::Vector4(0, 0, 0, 1));

	while (messagePump(window)) {
		renderer->clearFrame();

		lightPositions[0] = ecoliModel->getPosition();
		lightPositions[1] = herpesModel->getPosition();
		lightPositions[2] = malariaModel->getPosition();
		lightPositions[3] = poxModel->getPosition();

		//TODO: set color based on color saved for model or something of the like
		//the w value of the vector4 for the color specifies the light range
		
		lightColors[0].set(0.13, 0.94, 0.94, 100.0);
		lightColors[1].set(0.9, 0.9, 0.9, 100.0);
		lightColors[2].set(0.93, 0.13, 0.13, 100.0);
		lightColors[3].set(0.94, 0.13, 0.63, 100.0);

		renderer->setLightBuffers(lightPositions, lightColors, 4);

		frameCount++;
		if (renderer->getTimer()->GetFPSTime() > 1.0f)
		{
			fps = frameCount;
			frameCount = 0;
			renderer->getTimer()->ResetFPSTimer();
		}

		if (renderSelection) {
			renderer->getCamera()->set(Common::Vector4(0, 0, -10, 1), Common::Vector4(0, 0, 0, 1), Common::Vector4(0, 1, 0, 0));
			windowBgModel->draw();
			renderer->makeTransparent();
			titleModel->draw();
			renderer->makeOpaque();

			playerBgModel[0]->draw();
			playerModels[currModel]->setPosition(Common::Vector4(centers[0], 0, 0, 1));
			playerModels[currModel]->draw();

			for (int i = 1; i < 4; ++i) {
				playerBgModel[i]->draw();
				playerModels[i]->setPosition(Common::Vector4(centers[i], 0, 0, 1));
				playerModels[i]->draw();
			}
			newSelected = true;
		} else {
			poxModel->rotate(0.00f, 200.0f*renderer->getTimer()->GetCalculatedTimeSinceLastFrame(), 0.0f);

			tubeModel->draw();

			if (newSelected)
			{
				moreModels[currModel]->setPosition(Common::Vector4(0, 0, 0, 1));
			}
			moreModels[currModel]->draw();

			playerModels[1]->setPosition(7.5, 0, 35);
			playerModels[2]->setPosition(-20, 0, 35);
			playerModels[3]->setPosition(20, 0, 35);

			for (int i = 1; i < 4; ++i) {
				playerModels[i]->draw();
			}

			for (int i = 0; i < 4; ++i) {
				uiModels[i]->draw();
			}

			newSelected = false;
		}

		renderer->drawFrame();
		odprintf("FPS: %d", fps);
		odprintf("Time: %f", renderer->getTimer()->GetTime());

		moveBlob(window, moreModels[currModel], renderer->getCamera()); // temp input handler
		changeShader(window, tubeModel, defaultPixShad, pixShader);
	}

	delete ecoliModel;
	delete herpesModel;
	delete malariaModel;
	delete poxModel;
	delete tubeModel;
	delete titleModel;
	delete windowBgModel;

	for (int i = 0; i < 4; ++i) {
		delete playerModels[i];
		delete playerBgModel[i];
		delete uiModels[i];
	}

	delete defaultVertexShad;
	delete vertRipple;
	delete vertSpring;
	delete vertTent;
	delete vertTrack;
	delete vertWiggle;

	delete defaultPixShad;
	delete pixShader;
	delete pixBump;
	delete pixShaderNoSpec;
	delete pixCelShader;

	delete ecoliVbuf;
	delete ecoliIbuf;
	delete herpesVbuf;
	delete herpesIbuf;
	delete malariaVbuf;
	delete malariaIbuf;
	delete poxVbuf;
	delete poxIbuf;
	delete tubeVbuf;
	delete tubeIbuf;

	delete ecoliTex;
	delete herpesTex;
	delete malariaTex;
	delete poxTex;
	delete pipeTex;
	delete pipeBumpTex;
	delete titleTex;
	delete windowBgTex;
	delete playerBgTex;
	delete uiTex;

	delete renderer;
	delete window;

	//////////////////////////

	ecoliModel = nullptr;
	herpesModel = nullptr;
	malariaModel = nullptr;
	poxModel = nullptr;
	tubeModel = nullptr;
	titleModel = nullptr;
	windowBgModel = nullptr;

	for (int i = 0; i < 4; ++i) {
		playerModels[i] = nullptr;
		playerBgModel[i] = nullptr;
		uiModels[i] = nullptr;
	}

	defaultVertexShad = nullptr;
	vertRipple = nullptr;
	vertSpring = nullptr;
	vertTent = nullptr;
	vertTrack = nullptr;
	vertWiggle = nullptr;

	defaultPixShad = nullptr;
	pixShader = nullptr;
	pixBump = nullptr;
	pixShaderNoSpec = nullptr;
	pixCelShader = nullptr;

	ecoliVbuf = nullptr;
	ecoliIbuf = nullptr;
	herpesVbuf = nullptr;
	herpesIbuf = nullptr;
	malariaVbuf = nullptr;
	malariaIbuf = nullptr;
	poxVbuf = nullptr;
	poxIbuf = nullptr;
	tubeVbuf = nullptr;
	tubeIbuf = nullptr;

	ecoliTex = nullptr;
	herpesTex = nullptr;
	malariaTex = nullptr;
	poxTex = nullptr;
	pipeTex = nullptr;
	pipeBumpTex = nullptr;
	titleTex = nullptr;
	windowBgTex = nullptr;
	playerBgTex = nullptr;
	uiTex = nullptr;

	renderer = nullptr;
	window = nullptr;
}
