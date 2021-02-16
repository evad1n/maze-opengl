#if !defined(GRAPHICS_H)
#define GRAPHICS_H

enum ViewType {
    TOP,
    PERSPECTIVE,
    RAT,
    VIEW_COUNT
};

// Simple color struct
struct COLOR {
    double r, g, b;
};

struct Point2 {
    double x, y;
};

void DrawCircle(double x1, double y1, double radius);
void DrawRectangle(double x1, double y1, double width, double height);
void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
void DrawLine(double x1, double y1, double x2, double y2);
void DrawText(double x, double y, const char* string);


#endif // GRAPHICS_H
