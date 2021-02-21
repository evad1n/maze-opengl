#if !defined(MAZE_H)
#define MAZE_H

#include <vector>
#include "graphics.h"

class Maze;
class Cell;

class Cell {
public:
    Maze* maze;
    bool visited;
    // If this cell is currently being recursed upon
    bool recursing;
    // Walls
    bool top, left, right, bot;

    Cell(Maze* maze);
    ~Cell();

    void Draw(int x, int y, int size, int thick, int height, unsigned char walls);
    // Draw corner blocks
    void DrawCorner(int x, int y, int thick, int height);
    void Reset();
};

class Maze {
    friend class Cell;
private:
    double offsetX, offsetY;
    double width, height;
    double maxWidth, maxHeight;
    double cellSize;
    double wallThickness, wallHeight;

    std::vector<std::vector<Cell>> cells;

public:
    // Are we currently removing walls?
    bool creating;

public:
    Maze();
    Maze(double width, double height, int cellSize);
    ~Maze();

    void Draw();

    void Reset();
    void Resize(int newCellSize);
    void RemoveWalls(bool draw = false);
    void RemoveWallsRecursive(int row, int col, bool draw);
    // Adds 2 exits randomly on the outer rim
    void AddExits();

    Point2 GetCellPosition(int xIdx, int yIdx);
    Point2 RandomCellPosition();

    bool OnWall(double x, double y, double radius) const;

    double GetCellSize();
};




#endif // MAZE_H
