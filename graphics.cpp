// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include "graphics.h"
#include "maze.h"
#include "rat.h"

// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;

bool keystates[256];

int cellSize = 50;

Point2 look;

Maze maze;
Rat rat;
ViewType currentView = TOP;

//
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 32; i++) {
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x, double y, double width, double height) {
	glBegin(GL_QUADS);
	glVertex2d(x, y);
	glVertex2d(x + width, y);
	glVertex2d(x + width, y + height);
	glVertex2d(x, y + height);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
	glBegin(GL_TRIANGLES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();
}

void DrawLine(double x1, double y1, double x2, double y2) {
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, const char* string) {
	void* font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}

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
		// Camera cycle
	case 'c':
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

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor3d(1, 0, 0);			  // foreground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
