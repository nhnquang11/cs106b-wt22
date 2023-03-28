#include "Plotter.h"
#include "strlib.h"
using namespace std;

/* Function prototypes */
void runPlotterScript(istream& input);
void processCommand(const Vector<string>& command, bool& penDown, double& x, double& y, PenStyle& pen);

/**
 * Function: runPlotterScript
 * Usage: runPlotterScript(input)
 * --------------------------------------------------
 * Draws a canvas based on a set of commands from the input stream.
 */
void runPlotterScript(istream& input) {
    PenStyle pen { 1.0, "black" };
    double x = 0.0;
    double y = 0.0;
    bool penDown = false;
    for (string line; getline(input, line);){
        Vector<string> command = stringSplit(line, " ");
        processCommand(command, penDown, x, y, pen);
    }
}

/**
 * Function: processCommand
 * Usage: processCommand(command, penDown, x, y, pen)
 * --------------------------------------------------
 * A helper function implements the instruction given
 * the command and canvas information.
 */
void processCommand(const Vector<string>& command, bool& penDown, double& x, double& y, PenStyle& pen) {
    string instruction = toLowerCase(command[0]);
    if (instruction == "pendown") {
        penDown = true;
    } else if (instruction == "penup") {
        penDown = false;
    } else if (instruction == "moveabs") {
        double x1 = stringToDouble(command[1]);
        double y1 = stringToDouble(command[2]);
        if (penDown) {
            drawLine(x, y, x1, y1, pen);
        }
        x = x1;
        y = y1;
    } else if (instruction == "moverel") {
        double dx = stringToDouble(command[1]);
        double dy = stringToDouble(command[2]);
        if (penDown) {
            drawLine(x, y, x + dx, y + dy, pen);
        }
        x += dx;
        y += dy;
    } else if (instruction == "pencolor") {
        pen.color = command[1];
    } else if (instruction == "penwidth") {
        double width = stringToDouble(command[1]);
        pen.width = width;
    }
}
