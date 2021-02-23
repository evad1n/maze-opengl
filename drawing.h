#if !defined(DRAWING_H)
#define DRAWING_H

#include <string>

void DrawCircle(double x1, double y1, double radius);
void DrawRectangle(double x, double y, double width, double height, std::string textureName = "");
void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
void DrawLine(double x1, double y1, double x2, double y2);
void DrawText(double x, double y, const char* string);
void Draw3DQuad(double x, double y, double z, double xSize, double ySize, double height, std::string textureName = "");


#endif // DRAWING_H
