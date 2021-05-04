#include <GL/freeglut.h>
#include <cmath>
#include "graphics.h"
#include "drawing.h"
#include "rat.h"

extern ViewType currentView;
extern double deltaTime;

Rat::Rat() {}

Rat::Rat(Maze* maze, double radius, double startX, double startY)
    : maze(maze), x(startX), y(startY), rot(0), radius(radius), speed(0.1), rotSpeed(0.5) {}

Rat::~Rat() {}

void Rat::Draw() {
    if (currentView == RAT)
        return;

    glPushMatrix();

    if (currentView == TOP) {
        glTranslated(this->x, this->y, -2);
    } else {
        glTranslated(this->x, this->y, 2);
    }
    glRotated(this->rot, 0, 0, 1);
    glScaled(this->radius / 6, this->radius / 6, 1);

    glColor3d(.5, .5, .7);
    glBegin(GL_POLYGON);
    glVertex3d(-4, 4, 2);
    glVertex3d(-1, 6, 2);
    glVertex3d(1, 6, 2);
    glVertex3d(4, 4, 2);
    glVertex3d(5, 1, 2);
    glVertex3d(3, -4, 2);
    glVertex3d(0, -6, 2);
    glVertex3d(-3, -4, 2);
    glVertex3d(-5, 1, 2);
    glEnd();

    // Tail
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3d(0, -6, 2);
    glVertex3d(0, -8, 0);
    glEnd();
    glLineWidth(1);

    glPopMatrix();

    if (currentView == PERSPECTIVE) {
        // Draw beacon
        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex3d(this->x, this->y, 0);
        glVertex3d(this->x, this->y, 1000);
        glEnd();
    }

    // Debug collision radius
    // glColor4d(1, 0, 0, 0.2);
    // DrawCircle(this->x, this->y, this->radius);
}


void Rat::Resize(double radius) {
    this->radius = radius;
}

void Rat::MoveForward() {
    double radians = (90 + this->rot) / 180 * M_PI;
    double dx = cos(radians);
    double dy = sin(radians);
    double speed = this->speed * (this->maze->GetCellSize() / 20);
    // Future pos
    double fX = this->x + (speed * dx * (deltaTime / 2));
    double fY = this->y + (speed * dy * (deltaTime / 2));

    if (!this->maze->OnWall(fX, fY, this->radius)) {
        this->x = fX;
        this->y = fY;
    } else if (!this->maze->OnWall(fX, this->y, this->radius)) {
        this->x = fX;
    } else if (!this->maze->OnWall(this->x, fY, this->radius)) {
        this->y = fY;
    }
}

void Rat::MoveBackward() {
    double radians = (90 + this->rot) / 180 * M_PI;
    double dx = cos(radians);
    double dy = sin(radians);
    double speed = this->speed * (this->maze->GetCellSize() / 20);
    // Future pos
    double fX = this->x - (speed * dx * (deltaTime / 2));
    double fY = this->y - (speed * dy * (deltaTime / 2));

    if (!this->maze->OnWall(fX, fY, this->radius)) {
        this->x = fX;
        this->y = fY;
    } else if (!this->maze->OnWall(fX, this->y, this->radius)) {
        this->x = fX;
    } else if (!this->maze->OnWall(this->x, fY, this->radius)) {
        this->y = fY;
    }
}

void Rat::StrafeLeft() {
    double radians = (180 + this->rot) / 180 * M_PI;
    double dx = cos(radians);
    double dy = sin(radians);
    double speed = this->speed * (this->maze->GetCellSize() / 20);
    // Future pos
    double fX = this->x + (speed * dx * (deltaTime / 2));
    double fY = this->y + (speed * dy * (deltaTime / 2));

    if (!this->maze->OnWall(fX, fY, this->radius)) {
        this->x = fX;
        this->y = fY;
    } else if (!this->maze->OnWall(fX, this->y, this->radius)) {
        this->x = fX;
    } else if (!this->maze->OnWall(this->x, fY, this->radius)) {
        this->y = fY;
    }
}

void Rat::StrafeRight() {
    double radians = (this->rot) / 180 * M_PI;
    double dx = cos(radians);
    double dy = sin(radians);
    double speed = this->speed * (this->maze->GetCellSize() / 20);
    // Future pos
    double fX = this->x + (speed * dx * (deltaTime / 2));
    double fY = this->y + (speed * dy * (deltaTime / 2));

    if (!this->maze->OnWall(fX, fY, this->radius)) {
        this->x = fX;
        this->y = fY;
    } else if (!this->maze->OnWall(fX, this->y, this->radius)) {
        this->x = fX;
    } else if (!this->maze->OnWall(this->x, fY, this->radius)) {
        this->y = fY;
    }
}

void Rat::SpinLeft() {
    this->rot += this->rotSpeed * (deltaTime / 2);
}

void Rat::SprinRight() {
    this->rot -= this->rotSpeed * (deltaTime / 2);
}

void Rat::SetPosition(double x, double y) {
    this->x = x;
    this->y = y;
}

void Rat::SetRandomPosition() {
    Point2 p = this->maze->RandomCellPosition();
    this->x = p.x;
    this->y = p.y;
}

void Rat::Display() {
    double z_level = 1;
    double radians = (90 + this->rot) / 180 * M_PI;
    double dx = cos(radians);
    double dy = sin(radians);
    double eyeX = this->x + dx * this->radius * 20;
    double eyeY = this->y + dy * this->radius * 20;
    gluLookAt(this->x + dx * this->radius * .9, this->y + dy * this->radius * .9, z_level, eyeX, eyeY, z_level, 0, 0, 1);
}