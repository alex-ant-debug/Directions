#include <iostream>
#include <cmath>

using namespace std;

class Point
{
    public:
    float x, y;
    Point(){}

    Point(float pointX, float pointY)
    {
        this->x = pointX;
        this->y = pointY;
    }

    float distanceTo(Point *secondPoint)
    {
        return sqrt(pow(abs((this->x) - (secondPoint->x)),2) + pow(abs((this->y) - (secondPoint->y)),2));
    }

    void get(void)
    {
        cout<<" x = "<<this->x<<" y = "<<this->y<<endl;
    }
};

