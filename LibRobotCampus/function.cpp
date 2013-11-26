#include "function.h"

Function::Function() : nbPoints(0)
{
}

void Function::clear()
{
    nbPoints = 0;
}

double Function::getXMax()
{
    if (nbPoints == 0) {
        return 0.0;
    }

    return points[nbPoints-1][0];
}

void Function::addPoint(double x, double y)
{
    if (nbPoints < FUNCTION_MAX_POINTS) {
        points[nbPoints][0] = x;
        points[nbPoints][1] = y;

        if (nbPoints > 0) {
            ds[nbPoints-1] = points[nbPoints][1] - points[nbPoints-1][1];
            ds[nbPoints-1] /= points[nbPoints][0] - points[nbPoints-1][0];
        }

        nbPoints++;
    }
}

double Function::get(double x)
{
    int i;
    
    for (i=0; i<nbPoints; i++) {
        if (points[i][0] >= x) {
            break;
        }
    }

    if (i == 0 || i == nbPoints) {
        return 0.0;
    }

    return points[i-1][1]+ds[i-1]*(x-points[i-1][0]);
}

double Function::getMod(double x)
{
    double maxX = getXMax();

    if (x < 0.0 || x > maxX) {
        x -= maxX*(int)(x/maxX);
    }
    
    return get(x);
}

