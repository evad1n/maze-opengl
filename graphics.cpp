#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/freeglut.h>
#include <iostream>
#include <map>
#include "graphics.h"
#include "maze.h"
#include "rat.h"

// Global Variables
double screen_x = 700;
double screen_y = 500;

double deltaTime = 0;

// Textures
const int num_textures = 3;
// Textures folder will be prefixed later
std::string imageFiles[num_textures] = { "container.jpg", "sand.jpg", "steel.jpg" };
std::map<std::string, unsigned int> texMap;
unsigned int texName[num_textures];

bool keystates[256];

int cellSize = 50;

Point2 look;

Maze maze;
Rat rat;
ViewType currentView = TOP;

void SetTopView(int w, int h) {
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, w + world_margin_x,
		-world_margin_y, h + world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspectiveView(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 40.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 2000000);
	glMatrixMode(GL_MODELVIEW);
}

//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void) {
	static double prevTime = 0;
	double curr = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = curr - prevTime;
	prevTime = curr;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (currentView) {
	case TOP:
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		break;
	case PERSPECTIVE:
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		gluLookAt(screen_x / 2, -250, 300, look.x, look.y - 30, 0, 0, 0, 1);
		break;
	case RAT:
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		rat.Display();
		break;
	}

	maze.Draw();
	rat.Draw();

	if (keystates['w'])
		rat.MoveForward();
	if (keystates['a'])
		rat.SpinLeft();
	if (keystates['s'])
		rat.MoveBackward();
	if (keystates['d'])
		rat.SprinRight();

	glutSwapBuffers();
	glutPostRedisplay();
}

// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyDown(unsigned char c, int x, int y) {
	keystates[c] = true;
	switch (c) {
	case 27: // escape character means to quit the program
		exit(0);
		break;
	case 'c': // Camera cycle
	{
		int next = currentView + 1;
		next %= VIEW_COUNT;
		currentView = (ViewType)next;
		switch (currentView) {
		case TOP:
			SetTopView(screen_x, screen_y);
			break;
		case PERSPECTIVE:
			SetPerspectiveView(screen_x, screen_y);
			break;
		case RAT:
			SetPerspectiveView(screen_x, screen_y);
			break;
		}
		break;
	}
	case 13:
		// Enter
		maze.Reset();
		maze.RemoveWalls();
		rat.SetRandomPosition();
		break;
	case 32:
		// Space
		maze.Reset();
		maze.RemoveWalls(true);
		rat.SetRandomPosition();
		break;
	default:
		return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}

void keyUp(unsigned char c, int x, int y) {
	keystates[c] = false;

	glutPostRedisplay();
}

// For the rest of the keys
void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		// Up
		maze.Resize(++cellSize);
		maze.Reset();
		rat.Resize(double(cellSize) / 3);
		break;
	case GLUT_KEY_DOWN:
		// Down
		maze.Resize(--cellSize);
		maze.Reset();
		rat.Resize(double(cellSize) / 3);
		break;
	case GLUT_KEY_LEFT:
		look.x -= 10;
		break;
	case GLUT_KEY_RIGHT:
		look.x += 10;
		break;
	default:
		return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}

// This callback function gets ccdalled by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h) {
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);

	switch (currentView) {
	case TOP:
		SetTopView(w, h);
		break;
	case PERSPECTIVE:
		SetPerspectiveView(w, h);
		break;
	case RAT:
		SetPerspectiveView(w, h);
		break;
	}
}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y) {
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
	}
	glutPostRedisplay();
}

void loadTextures() {
	glGenTextures(num_textures, texName);

	for (int i = 0; i < num_textures; i++) {
		texMap[imageFiles[i]] = i;
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(("textures/" + imageFiles[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
			// NOTE: If the above command doesn't work, try it this way:
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				//glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
}

// Your initialization code goes here.
void InitializeMyStuff() {
	srand(time(NULL));
	maze = Maze(screen_x, screen_y, cellSize);
	rat = Rat(&maze, double(cellSize) / 3, 0, 0);
	rat.SetRandomPosition();
	look = Point2{ screen_x / 2, screen_y / 2 };

	// Textures
	// int numTex = 10;
	// char imageFiles[numTex][100] = {};
	// glGenTextures(numTex, texName);

	// for (int i = 0; i < numTex; i++) 	{
	// 	glBindTexture(GL_TEXTURE_2D, texName[i]);
	// 	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	// 	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// 	int width, height, nrChannels;
	// 	unsigned char* data = stbi_load
	// }
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) {
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} else {
		glutCreateWindow("Maze Runner");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	// Enable transparency from alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glColor3d(1, 0, 0);			  // foreground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}