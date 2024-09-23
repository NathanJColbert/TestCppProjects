#include <iostream>
#include <string>

struct Point {
    Point(int xPos, int yPos) : x(xPos), y(yPos) { }
    int x, y;
};
struct Line {
    Line(Point point1, Point point2) : a(Point(point1)), b(Point(point2)) { }
    Point a;
    Point b;
};

Point getInputPoint() {
    int x, y;
    std::cin >> x;
    std::cin >> y;
    return Point(x, y);
}

void print(const Point& p) {
    std::cout << '(' << p.x << ", " << p.y << ")\n";
}


int main() {
    
    Line line = Line(getInputPoint(), getInputPoint());
    Point point = getInputPoint();

    // determinant = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
    int i = (line.b.x - line.a.x) * (point.y - line.a.y) - (line.b.y - line.a.y) * (point.x - line.a.x);

    if (i == 0) std::cout << "SAME\n";
    else if (i < 0) std::cout << "LEFT\n";
    else std::cout << "RIGHT\n";

    return 0;
}