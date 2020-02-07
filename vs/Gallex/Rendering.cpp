#include "Rendering.h"

Rendering::Rendering()
{
	preloader = PreLoader::GetInstance();
	preloader->LoadFont("font");

	scene = SpaceScene::GetInstance();

	glutSetCursor(GLUT_CURSOR_NONE);
	mouseButton = NONE_BTN_MOUSE;
	gxCursor = new GxCursor();
	gxCursor->SetSize(27);

	gui = new GUI();

	startTime = glutGet(GLUT_ELAPSED_TIME);
}

void Rendering::Display()
{
	if (scene->GetDeadScene())
	{
		delete scene;
		delete gxCursor;
		delete gui;
		delete preloader;
		exit(0);
	}
		
	Render(GL_RENDER);
	glutSwapBuffers();

	frameCount++;

	long deltaT = (glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000;

	if (deltaT > 0)
	{
		gui->SetFPS(frameCount / deltaT);
		startTime = glutGet(GLUT_ELAPSED_TIME);
		frameCount = 0;
	}
}

void Rendering::Render(GLuint mode)
{
	glViewport(0, 0, preloader->GetScreen().width, preloader->GetScreen().height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
			
	switch (mode)
	{	
		case GL_SELECT:
			scene->SimpleDraw();
			break;
		case GL_RENDER:
			scene->Update();
			scene->MoveCamera(gxCursor->GetEdge());
			scene->Draw();	

			string cm = scene->GetCollideMessage();
			if (cm != "")
				gui->ShowDlgBox("Oops!", cm, ZERO_LVL);
						
			gui->Draw();

			gxCursor->Draw();
								
			break;
	}
}

void Rendering::Resize(const int width, const int height)
{
	preloader->SetScreen(width, height, preloader->GetScreen().depth, preloader->GetScreen().frequency);
	scene->Resize(width, height);
}

void Rendering::Destroy()
{
	preloader->UnloadTextures();
	preloader->UnloadShaders();
}

void Rendering::OnMouseButtonUp(int x, int y, int btn)
{
	lastX = x;
	lastY = y;
	mouseButton = NONE_BTN_MOUSE;
		
	gui->MouseClick(x, y);
}

void Rendering::OnMouseButtonDown(int x, int y, int btn)
{
	lastX = x;
	lastY = y;
	mouseButton = btn;

	if (btn == LEFT_BTN_MOUSE && gui->ZerroMode())
		ProcessColorBuffer(x, y);
}

void Rendering::OnMouseMove(int x, int y)
{
	gxCursor->SetPosition(x, y);

	gui->MouseMove(x, y);

	if (mouseButton == RIGHT_BTN_MOUSE && gui->ZerroMode())
	{
		scene->UpdateEulerAngles();		
		scene->RotateCamera(lastX - x, lastY - y);

		lastX = x;
		lastY = y;
	}

	if (mouseButton == MIDDLE_BTN_MOUSE && gui->ZerroMode())
	{
		scene->ZoomCamera(lastY - y);
		lastY = y;
	}		
}

void Rendering::OnKeyPressed(int key)
{
	switch (key)
	{
	case 27:
		gui->PressedEsc();
		break;
	case 13:
		gui->PressedEnter();
		break;
	case 38:
		gui->PressedKeyUp();
		break;
	case 40:
		gui->PressedKeyDown();
		break;
	case 8:
		gui->PressedBackspace();
		break;
	case 114:
		if (gui->ZerroMode())
			scene->ResetCameraPosition();
		break;
	case 102:
		if(gui->ZerroMode())
			scene->FocusOnSelectedObject();
		break;
	case 105:
		gui->SwitchShowInfo();
		break;
	}

	if (key >= 32 && key <= 128)
	{
		gui->PressedSymbol(key);
	}
}

void Rendering::ProcessColorBuffer(int x, int y)
{
	vec3b color;

	Render(GL_SELECT);	
	glReadPixels(x, preloader->GetScreen().height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color);
	Render(GL_RENDER);	
	scene->SelectByColor(color);
	gui->LoadInfo();
}

void Rendering::SetTimeScale(const int ms)
{
	scene->SetTimeIncrement((float)ms / 1000.0f);
}