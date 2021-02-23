#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include <map>
#include <iostream>
#include "drawing.h"

extern std::map<std::string, unsigned int> texMap;

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

void DrawRectangle(double x, double y, double width, double height, std::string textureName) {
    if (textureName != "") {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texMap[textureName]);

        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2d(x, y);
        glTexCoord2f(1, 1); glVertex2d(x + width, y);
        glTexCoord2f(1, 0); glVertex2d(x + width, y + height);
        glTexCoord2f(0, 0); glVertex2d(x, y + height);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    } else {
        glBegin(GL_QUADS);
        glVertex2d(x, y);
        glVertex2d(x + width, y);
        glVertex2d(x + width, y + height);
        glVertex2d(x, y + height);
        glEnd();
    }
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

// 3D Quad
// x,y,z are the lower coordinate values (bottom left lowest corner)
void Draw3DQuad(double x, double y, double z, double xSize, double ySize, double height, std::string textureName) {
    if (textureName != "") {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texMap[textureName]);

        // -z
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3d(x, y, z);
        glTexCoord2f(1, 1); glVertex3d(x, y + ySize, z);
        glTexCoord2f(1, 0); glVertex3d(x + xSize, y, z);
        glTexCoord2f(0, 0); glVertex3d(x + xSize, y + ySize, z);
        glEnd();
        // +z
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3d(x, y, z + height);
        glTexCoord2f(1, 1); glVertex3d(x, y + ySize, z + height);
        glTexCoord2f(1, 0); glVertex3d(x + xSize, y + ySize, z + height);
        glTexCoord2f(0, 0); glVertex3d(x + xSize, y, z + height);
        glEnd();
        // -x
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3d(x, y, z);
        glTexCoord2f(1, 1); glVertex3d(x, y + ySize, z);
        glTexCoord2f(1, 0); glVertex3d(x, y + ySize, z + height);
        glTexCoord2f(0, 0); glVertex3d(x, y, z + height);
        glEnd();
        // +x
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3d(x + xSize, y, z);
        glTexCoord2f(1, 1); glVertex3d(x + xSize, y + ySize, z);
        glTexCoord2f(1, 0); glVertex3d(x + xSize, y + ySize, z + height);
        glTexCoord2f(0, 0); glVertex3d(x + xSize, y, z + height);
        glEnd();
        // -y
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);glVertex3d(x, y, z);
        glTexCoord2f(1, 1);glVertex3d(x + xSize, y, z);
        glTexCoord2f(1, 0);glVertex3d(x + xSize, y, z + height);
        glTexCoord2f(0, 0);glVertex3d(x, y, z + height);
        glEnd();
        // +y
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);glVertex3d(x, y + ySize, z);
        glTexCoord2f(1, 1);glVertex3d(x + xSize, y + ySize, z);
        glTexCoord2f(1, 0);glVertex3d(x + xSize, y + ySize, z + height);
        glTexCoord2f(0, 0);glVertex3d(x, y + ySize, z + height);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    } else {
        // -z
        glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x, y + ySize, z);
        glVertex3d(x + xSize, y, z);
        glVertex3d(x + xSize, y + ySize, z);
        glEnd();
        // +z
        glBegin(GL_QUADS);
        glVertex3d(x, y, z + height);
        glVertex3d(x, y + ySize, z + height);
        glVertex3d(x + xSize, y + ySize, z + height);
        glVertex3d(x + xSize, y, z + height);
        glEnd();
        // -x
        glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x, y + ySize, z);
        glVertex3d(x, y + ySize, z + height);
        glVertex3d(x, y, z + height);
        glEnd();
        // +x
        glBegin(GL_QUADS);
        glVertex3d(x + xSize, y, z);
        glVertex3d(x + xSize, y + ySize, z);
        glVertex3d(x + xSize, y + ySize, z + height);
        glVertex3d(x + xSize, y, z + height);
        glEnd();
        // -y
        glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x + xSize, y, z);
        glVertex3d(x + xSize, y, z + height);
        glVertex3d(x, y, z + height);
        glEnd();
        // +y
        glBegin(GL_QUADS);
        glVertex3d(x, y + ySize, z);
        glVertex3d(x + xSize, y + ySize, z);
        glVertex3d(x + xSize, y + ySize, z + height);
        glVertex3d(x, y + ySize, z + height);
        glEnd();
    }
}

//     // Draw quads with texture
//     glEnable(GL_TEXTURE_2D);

//     glBindTexture(GL_TEXTURE_2D, texName[0]); // Sandra

//     glBegin(GL_QUADS);
//     glTexCoord2f(0, 1); glVertex2d(-1, 0); // All of Sandra
//     glTexCoord2f(1, 1); glVertex2d(5, 0);
//     glTexCoord2f(1, 0); glVertex2d(5, 8);
//     glTexCoord2f(0, 0); glVertex2d(-1, 8);
//     glEnd(); // GL_QUADS

//     glBegin(GL_QUADS);
//     glTexCoord2f(.2, .7); glVertex2d(5.1, 5); // Part of Sandra
//     glTexCoord2f(.8, .5); glVertex2d(8.1, 5);
//     glTexCoord2f(.6, .0); glVertex2d(8.1, 8);
//     glTexCoord2f(.0, .2); glVertex2d(5.1, 8);
//     glEnd(); // GL_QUADS

//     glBindTexture(GL_TEXTURE_2D, texName[1]); // Brick

//     glBegin(GL_QUADS);
//     glTexCoord2f(0, 1); glVertex2d(5.1, 2); // All of brick
//     glTexCoord2f(1, 1); glVertex2d(10.1, 2);
//     glTexCoord2f(1, 0); glVertex2d(10.1, 5);
//     glTexCoord2f(0, 0); glVertex2d(5.1, 5);
//     glEnd(); // GL_QUADS

//     glBegin(GL_QUADS);
//     glTexCoord2f(0, 3); glVertex2d(5.1, -1); // Repeating brick in 3x3 pattern
//     glTexCoord2f(3, 3); glVertex2d(10.1, -1);
//     glTexCoord2f(3, 0); glVertex2d(10.1, 2);
//     glTexCoord2f(0, 0); glVertex2d(5.1, 2);
//     glEnd(); // GL_QUADS

//     glDisable(GL_TEXTURE_2D);

//     glutSwapBuffers();
// }