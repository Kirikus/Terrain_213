#include "screenangle.h"

// The function simulates the relief
double ModelReliefFunction(double x, double y)
{
    return -sqrt(pow((x-20), 2) + pow(y, 2)) + 15;
}
