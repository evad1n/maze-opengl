#if !defined(RAT_H)
#define RAT_H

#include "maze.h"

class Rat {
private:
    Maze* maze;
    double x, y, rot;
    double radius;
    double speed, rotSpeed;
public:
    Rat();
    Rat(Maze* maze, double radius, double startX, double startY);
    ~Rat();

    void Draw();
    void Resize(double radius);

    void MoveForward();
    void MoveBackward();
    void SpinLeft();
    void SprinRight();

    void SetPosition(double x, double y);
    void SetRandomPosition();

    void Display();
};


#endif // RAT_H
