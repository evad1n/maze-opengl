#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include "graphics.h"
#include "drawing.h"
#include "maze.h"

extern ViewType currentView;
const int mazeOffset = 20;

enum Walls {
    Top = 0x01,
    Left = 0x02,
    Right = 0x04,
    Bot = 0x08
};


Cell::Cell(Maze* maze)
    : maze(maze), visited(false), recursing(false) {
    top = left = right = bot = true;
}

Cell::~Cell() {
    // delete this->maze;
}

void Cell::Draw(int x, int y, int size, double thick, double height, unsigned char walls) {
    // Draw floors
    DrawRectangle(x, y, size, size, "sand.jpg");

    // Draw shading overlays
    glPushMatrix();
    glTranslated(0, 0, 1);
    if (this->visited) {
        if (this->recursing) {
            glColor4ub(255, 0, 0, 40);
            DrawRectangle(x, y, size, size);
        }
    } else {
        glColor4ub(0, 0, 0, 200);
        DrawRectangle(x, y, size, size);
    }
    glPopMatrix();

    if (currentView == TOP) {

        std::cout << thick << std::endl;

        glColor3d(0, 0, 0);
        if (this->top && (walls & Walls::Top))
            DrawRectangle(x + thick / 2, y + size - thick / 2, size - thick, thick);
        if (this->left)
            DrawRectangle(x - thick / 2, y + thick / 2, thick, size - thick);
        if (this->right && (walls & Walls::Right))
            DrawRectangle(x + size - thick / 2, y + thick / 2, thick, size - thick);
        if (this->bot)
            DrawRectangle(x + thick / 2, y - thick / 2, size - thick, thick);

        this->DrawCorner(x, y, thick, 0);

        if (walls & Walls::Top) {
            this->DrawCorner(x, y + size, thick, height);
        }
        if (walls & Walls::Right) {
            this->DrawCorner(x + size, y, thick, height);
        }
        if ((walls & Walls::Top) && (walls & Walls::Right)) {
            this->DrawCorner(x + size, y + size, thick, height);
        }

        // Draw floors
        // glColor4ub(0, 255, 0, 100);
        // DrawRectangle(x, y, size, size);
    } else {
        if (this->top && (walls & Walls::Top)) {
            int r = (x * 13233 + y * 3235) % 256;
            int g = (x * 15364 + y * 21316) % 256;
            int b = (x * 12782 + y * 213123) % 256;
            glColor3ub(r, g, b);
            Draw3DQuad(x + thick / 2, y + size - thick / 2, 0, size - thick, thick, height, "container.jpg");
        }
        if (this->left) {
            int r = (x * 17495 + y * 7867) % 256;
            int g = (x * 19626 + y * 212316) % 256;
            int b = (x * 21757 + y * 21316) % 256;
            glColor3ub(r, g, b);
            Draw3DQuad(x - thick / 2, y + thick / 2, 0, thick, size - thick, height, "container.jpg");
        }
        if (this->right && (walls & Walls::Right)) {
            int r = (x * 23888 + y * 21316) % 256;
            int g = (x * 26019 + y * 29876) % 256;
            int b = (x * 28150 + y * 21316) % 256;
            glColor3ub(r, g, b);
            Draw3DQuad(x + size - thick / 2, y + thick / 2, 0, thick, size - thick, height, "container.jpg");
        }
        if (this->bot) {
            int r = (x * 30281 + y * 2424) % 256;
            int g = (x * 32412 + y * 2138) % 256;
            int b = (x * 239012 + y * 123323) % 256;
            glColor3ub(r, g, b);
            Draw3DQuad(x + thick / 2, y - thick / 2, 0, size - thick, thick, height, "container.jpg");
        }

        this->DrawCorner(x, y, thick, height);

        if (walls & Walls::Top) {
            this->DrawCorner(x, y + size, thick, height);
        }
        if (walls & Walls::Right) {
            this->DrawCorner(x + size, y, thick, height);
        }
        if ((walls & Walls::Top) && (walls & Walls::Right)) {
            this->DrawCorner(x + size, y + size, thick, height);
        }
    }
}

void Cell::DrawCorner(int centerX, int centerY, double thick, double height) {
    if (currentView == TOP) {
        glColor3ub(0, 0, 0);
        DrawRectangle(centerX - thick / 2, centerY - thick / 2, thick, thick);
    } else {
        thick *= 1.2;
        height *= 1.5;
        glColor3ub(0, 0, 0);
        Draw3DQuad(centerX - thick / 2, centerY - thick / 2, 0, thick, thick, height, "steel.jpg");
    }
}


void Cell::Reset() {
    top = left = right = bot = true;
    visited = recursing = false;
}

Maze::Maze() {}

Maze::Maze(double width, double height, int cellSize)
    : cellSize(cellSize) {
    this->maxWidth = width - (mazeOffset * 2);
    this->maxHeight = height - (mazeOffset * 2);

    // Initialize cells
    this->cells = std::vector<std::vector<Cell>>{ };

    this->Resize(cellSize);
    // Not carved out yet
    this->creating = true;
}

Maze::~Maze() {}

void Maze::Draw() {
    for (int x = 0; x < this->cells.size(); x++) {
        for (int y = 0; y < this->cells[0].size(); y++) {
            unsigned char walls = 0;
            if (x == this->cells.size() - 1)
                walls |= Walls::Right;
            if (y == this->cells[0].size() - 1)
                walls |= Walls::Top;
            this->cells[x][y].Draw(this->offsetX + x * this->cellSize, this->offsetY + y * this->cellSize, this->cellSize, this->wallThickness, this->wallHeight, walls);
        }
    }
}

void Maze::Reset() {
    this->creating = false;
    for (int x = 0; x < this->cells.size(); x++) {
        for (int y = 0; y < this->cells[0].size(); y++) {
            this->cells[x][y].Reset();
        }
    }
}

void Maze::Resize(int cellSize) {
    this->creating = false;
    this->cellSize = cellSize;
    this->wallThickness = (double)cellSize / 10.0;
    this->wallHeight = (double)cellSize / 10.0;

    // Set size
    double cols = int(this->maxWidth / cellSize);
    double rows = int(this->maxHeight / cellSize);
    double xDiff = this->maxWidth - (cols * cellSize);
    double yDiff = this->maxHeight - (rows * cellSize);

    this->width = cols * cellSize;
    this->height = rows * cellSize;

    this->offsetX = mazeOffset + xDiff / 2;
    this->offsetY = mazeOffset + yDiff / 2;

    // Clear cells
    this->cells.clear();
    for (int x = 0; x < cols; x++) {
        std::vector<Cell> col{};
        for (int y = 0; y < rows; y++) {
            col.push_back(Cell(this));
        }
        this->cells.push_back(col);
    }
}

void Maze::RemoveWalls(bool draw) {
    this->creating = true;
    this->RemoveWallsRecursive(rand() % this->cells.size(), rand() % this->cells[0].size(), draw);
    this->AddExits();
    this->creating = false;
}


void Maze::RemoveWallsRecursive(int x, int y, bool draw) {
    if (!this->creating)
        return;
    this->cells[x][y].visited = true;
    this->cells[x][y].recursing = true;
    enum NEIGHBORS { LEFT, RIGHT, TOP, BOT };
    while (this->creating) {
        if (draw) {
            this->Draw();
            glutSwapBuffers();
            glutPostRedisplay();
            // Ask for events so this is non blocking
            glutMainLoopEvent();
            if (!this->creating)
                return;
        }
        std::vector<NEIGHBORS> n;
        if (x > 0 && !this->cells[x - 1][y].visited) {
            n.push_back(LEFT);
        }
        if (x < this->cells.size() - 1 && !this->cells[x + 1][y].visited) {
            n.push_back(RIGHT);
        }
        if (y < this->cells[0].size() - 1 && !this->cells[x][y + 1].visited) {
            n.push_back(TOP);
        }
        if (y > 0 && !this->cells[x][y - 1].visited) {
            n.push_back(BOT);
        }

        if (n.size() == 0) {
            this->cells[x][y].recursing = false;
            return;
        }

        int r = rand() % n.size();

        if (n[r] == LEFT) {
            this->cells[x][y].left = false;
            this->cells[x - 1][y].right = false;
            this->RemoveWallsRecursive(x - 1, y, draw);
            if (!this->creating)
                return;
        } else if (n[r] == RIGHT) {
            this->cells[x][y].right = false;
            this->cells[x + 1][y].left = false;
            this->RemoveWallsRecursive(x + 1, y, draw);
            if (!this->creating)
                return;
        } else if (n[r] == TOP) {
            this->cells[x][y].top = false;
            this->cells[x][y + 1].bot = false;
            this->RemoveWallsRecursive(x, y + 1, draw);
            if (!this->creating)
                return;
        } else {
            this->cells[x][y].bot = false;
            this->cells[x][y - 1].top = false;
            this->RemoveWallsRecursive(x, y - 1, draw);
            if (!this->creating)
                return;
        }
    }
}

void Maze::AddExits() {
    int exit1 = rand() % this->cells[0].size();
    int exit2 = rand() % this->cells[0].size();

    this->cells[0][exit1].left = false;
    this->cells[this->cells.size() - 1][exit2].right = false;
}

// Collision handling
bool Maze::OnWall(double x, double y, double radius) const {
    int mazeX = (x - this->offsetX);
    int mazeY = (y - this->offsetY);
    int xCell = mazeX / this->cellSize;
    int yCell = mazeY / this->cellSize;
    if (mazeX < 0 || xCell > this->cells.size() - 1 || mazeY < 0 || yCell > this->cells[0].size() - 1) {
        return false;
    }

    radius += this->wallThickness / 2;

    double offsetX = x - (this->offsetX + (xCell * this->cellSize));
    double offsetY = y - (this->offsetY + (yCell * this->cellSize));

    // glColor3d(0, 1, 0);
    // DrawRectangle(this->offsetX + xCell * this->cellSize, this->offsetX + yCell * this->cellSize, this->cellSize, this->cellSize);

    // Walls
    if (this->cells[xCell][yCell].left && offsetX - radius < 0)
        return true;
    if (this->cells[xCell][yCell].right && offsetX + radius > this->cellSize)
        return true;
    if (this->cells[xCell][yCell].top && offsetY + radius > this->cellSize)
        return true;
    if (this->cells[xCell][yCell].bot && offsetY - radius < 0)
        return true;

    // Corners

    // top-left
    if (offsetY + radius > this->cellSize && offsetX - radius < 0)
        return true;
    // top-right
    if (offsetY + radius > this->cellSize && offsetX + radius > this->cellSize)
        return true;
    // bot-left
    if (offsetY - radius < 0 && offsetX - radius < 0)
        return true;
    // bot-right
    if (offsetY - radius < 0 && offsetX + radius > this->cellSize)
        return true;

    return false;
}

Point2 Maze::GetCellPosition(int xIdx, int yIdx) {
    return Point2{
        this->offsetX + (xIdx * this->cellSize),
        this->offsetY + (yIdx * this->cellSize),
    };
}

Point2 Maze::RandomCellPosition() {
    int x = rand() % this->cells.size();
    int y = rand() % this->cells[0].size();
    return Point2{
        this->offsetX + (x * this->cellSize) + this->cellSize / 2,
        this->offsetY + (y * this->cellSize) + this->cellSize / 2,
    };
}

double Maze::GetCellSize() {
    return this->cellSize;
}
