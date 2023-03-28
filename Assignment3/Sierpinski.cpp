#include "Sierpinski.h"
#include "error.h"
using namespace std;

/**
 * @param window The window in which to draw the triangle.
 * @param x0 y0 The first corner of the triangle.
 * @param x1 y1 The second corner of the triangle.
 * @param x2 y2 The third corner of the triangle.
 */
void drawTriangle(GWindow& window,
                  double x0, double y0,
                  double x1, double y1,
                  double x2, double y2) {
    window.setColor("black");
    window.fillPolygon({ x0, y0, x1, y1, x2, y2 });
}

void drawSierpinskiTriangle(GWindow& window,
                            double x0, double y0,
                            double x1, double y1,
                            double x2, double y2,
                            int order) {
    if (order < 0) error("Invalid order! The order must be greater than or equal to 0.");
    if (order == 0) {
        drawTriangle(window, x0, y0, x1, y1, x2, y2);
    } else {
        double x01 = (x0 + x1) / 2;
        double y01 = (y0 + y1) / 2;
        double x12 = (x1 + x2) / 2;
        double y12 = (y1 + y2) / 2;
        double x02 = (x0 + x2) / 2;
        double y02 = (y0 + y2) / 2;
        drawSierpinskiTriangle(window, x0, y0, x01, y01, x02, y02, order - 1);
        drawSierpinskiTriangle(window, x1, y1, x01, y01, x12, y12, order - 1);
        drawSierpinskiTriangle(window, x2, y2, x02, y02, x12, y12, order - 1);
    }
}
