#include "Gallex.h"

#define FPS 60

void timer(int value) {
	glutPostRedisplay(); 
	glutTimerFunc(preloader->GetRefreshMills(), timer, 0);
}

void Display()
{	
	rendering->Display();
}

void Reshape(int width, int height)
{
	rendering->Resize(width, height);
}

void Keyboard(unsigned char key, int x, int y)
{
	rendering->OnKeyPressed(key);
}

void KeyboardSpecial(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			rendering->OnKeyPressed(38);
			break;
		case GLUT_KEY_DOWN:
			rendering->OnKeyPressed(40);
			break;		
	}	
}

void MouseMotion(int x, int y)
{
	rendering->OnMouseMove(x, y);
}

void MouseAction(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		switch (state)
		{
		case GLUT_DOWN:
			rendering->OnMouseButtonDown(x, y, LEFT_BTN_MOUSE);
			break;
		case GLUT_UP:
			rendering->OnMouseButtonUp(x, y, LEFT_BTN_MOUSE);
			break;
		}
	}

	if (button == GLUT_RIGHT_BUTTON)
	{
		switch (state)
		{
			case GLUT_DOWN:
				rendering->OnMouseButtonDown(x, y, RIGHT_BTN_MOUSE);
				break;
			case GLUT_UP:
				rendering->OnMouseButtonUp(x, y, RIGHT_BTN_MOUSE);
				break;
		}
	}

	if (button == 1)
	{
		switch (state)
		{
		case GLUT_DOWN:
			rendering->OnMouseButtonDown(x, y, MIDDLE_BTN_MOUSE);
			break;
		case GLUT_UP:
			rendering->OnMouseButtonUp(x, y, MIDDLE_BTN_MOUSE);
			break;
		}
	}	
}

int main(int argc, char* argv[])
{
	printf("Starting...\n"); 

	preloader = PreLoader::GetInstance();
	preloader->LoadConfig("settings.conf");
	
	char config[50];

	sprintf_s(config, "%dx%d:%d@%d", preloader->GetScreen().width, preloader->GetScreen().height, preloader->GetScreen().depth, preloader->GetScreen().frequency);
	printf("Display: %s\n", config);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutGameModeString(config);
	glutEnterGameMode();

	GLenum err = glewInit();

	if (GLEW_OK != err)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	else
		fprintf(stdout, "GLEW: %s\n", glewGetString(GLEW_VERSION));

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslver = glGetString(GL_SHADING_LANGUAGE_VERSION);

	cout << "Renderer: " << renderer << endl;
	cout << "Vendor: " << vendor << endl;
	cout << "Version: " << version << endl;
	cout << "Shading language ver.: " << glslver << endl;

	preloader->SetRefreshMills(1000/FPS);

	rendering = new Rendering();
	rendering->SetTimeScale(preloader->GetRefreshMills());

	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MouseMotion);
	glutMouseFunc(MouseAction);
	glutReshapeFunc(Reshape);
	glutTimerFunc(1000/FPS, timer, 0);
	glutMainLoop();
}