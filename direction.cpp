#include <iostream>
#include <vector>
#include <cstring>
#include "point.cpp"

using namespace std;

bool myIsNumber(string str)
{
    bool flagFindingPoint = 0;
    const char *pstr = str.c_str();
    unsigned int j;
    j = (str[0] == '-') ? 1 : 0;
    for(;j<strlen(pstr);j++)
    {
        if(str[j] != '.')
        {
            if(!isdigit(str[j]))
            {
                return 0;
            }
        }
        else
        {
            if(j == 0)
            {
                return 0;
            }
            else
            {
               if(flagFindingPoint == 1)
               {
                   return 0;
               }
            }
            flagFindingPoint = 1;
        }
    }
    return 1;
}

class Direction
{
    public:

    static const int wideAngle = 180;

    static bool validateRowData(vector<string> parsedData)
    {
        int vectorSize = parsedData.size();
        if(vectorSize < 6)                                  //min size 6
        {
            return 0;
        }

        int multiplicity = (vectorSize-2)%4;                //multiplicity of four
        if(multiplicity != 0)
        {
            return 0;
        }

        if(!myIsNumber(parsedData[0]))                      //x  is nomber
        {
            return 0;
        }

        if(!myIsNumber(parsedData[1]))                      //y is nomber
        {
            return 0;
        }

        for(int j=2;j<vectorSize;j+=4)
        {
            if(j == 2 && parsedData[j] != "start")
            {
                return 0;
            }

            if(j != 2 && parsedData[j] != "turn")
            {
                return 0;
            }

            if(!myIsNumber(parsedData[j+1]))            //angle is nomber
            {
                return 0;
            }

            float angle = stof(parsedData[j+1]);
            if((angle < -360)||(angle > 360))           // -360 < angle < 360
            {
                return 0;
            }
            if(parsedData[j+2] != "walk")               //walk
            {
                return 0;
            }

            // проверить, что расстояние число
            if(!myIsNumber(parsedData[j+3]))            //walk is nomber
            {
                return 0;
            }

            float walk =  stof(parsedData[j+3]);
            if(walk < 0)
            {
                return 0;
            }
        }

        return 1;
    }

//    static bool validateRowData(vector<string> parsedData)
//    {
//        int vectorSize = parsedData.size();
//        int multiplicity = (vectorSize-2)%4;                                //multiplicity of four
//        if(multiplicity == 0)
//        {
//            if(myIsNumber(parsedData[0]))                                   //x  is nomber
//            {
//                if(myIsNumber(parsedData[1]))                               //y is nomber
//                {
//                    for(int j=2;j<vectorSize;j+=4)
//                    {
//                        if((parsedData[j] == "start")||(parsedData[j] == "turn"))//start & turn
//                        {
//                            float isAngle = stof(parsedData[j+1]);
//                            if((isAngle >= -360)&&(isAngle <= 360))             // -360 < angle < 360
//                            {
//                                if(parsedData[j+2] == "walk")                   //walk
//                                {
//                                    float isWalk =  stof(parsedData[j+3]);
//                                    if(isWalk >= 0)
//                                    {
//                                        if((j+4) == vectorSize)
//                                        {
//                                            return 1;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//        return 0;
//    }

    static bool validateData(vector<vector<string>> parsedData)
    {
        int n = parsedData.size();

        unsigned int count = 0;

        for(int i = 0; i< n; i++)
        {
            if(count == 0)
            {
                int firstRowSize = parsedData[i].size();
                if((firstRowSize != 1))                   // первый вектор единичный
                {
                    return 0;
                }

                if(!myIsNumber(parsedData[i][0]))
                {
                    return 0;
                }

                count = stof(parsedData[i][0]);

                if(count > (n-i-1))                 //если count больше чем кол. строк
                {
                    return 0;
                }

                if((count == 0)&&(i != n-1))
                {
                    return 0;
                }
            }
            else
            {
                count--;
            }
        }

        return 1;
    }

    static void runTasks(vector<vector<string>> parsedData)
    {
        int n = parsedData.size();

        unsigned int count = 0 ;
        Direction direction;

        for(int i = 0; i< n; i++)
        {
            if(count == 0)
            {
                if(i != 0)
                {
                   if(direction.points.size() > 0)
                    {
                        direction.calculateTotals();
                    }
                }

                direction = Direction();
                count = stof(parsedData[i][0]);
                cout<<" "<<endl;
            }
            else
            {
                direction.calculateTargetPoint(parsedData[i]);
                count--;
            }
        }
    }

    void calculateTargetPoint(vector<string> directionsData)
    {
        float walk, angle, x, y;

        if(validateRowData(directionsData))
        {
            int vectorSize = directionsData.size();
            x = stof(directionsData[0]);
            y = stof(directionsData[1]);
            angle = 0;

            for(int j=3;j<vectorSize;j+=4)
            {
                angle += stof(directionsData[j]);
                walk = stof(directionsData[j+2]);

                x += walk*cos((angle*M_PI)/wideAngle);
                y += walk*sin((angle*M_PI)/wideAngle);
            }

            Point* point;
            point = new Point(x, y);
            this->points.push_back(point);
        }
        else
        {
            cout<<"False string in file"<<endl;//throw 100;
        }
    }

    void calculateTotals(void)
    {
        float midPointX = 0, midPointY = 0;
        unsigned int pointsSize = this->points.size();
        for(unsigned int k=0;k<pointsSize;k++)
        {
            midPointX += this->points[k]->x;
            midPointY += this->points[k]->y;
        }

        midPointX /= this->points.size();
        midPointY /= this->points.size();
        cout<<"midX = "<<midPointX<<" midY = "<<midPointY<<endl;

        Point midPoint(midPointX, midPointY);
        float maxDistance = 0;
        for(unsigned int h=0;h<pointsSize;h++)
        {
            if(maxDistance < midPoint.distanceTo(this->points[h]))
            {
                maxDistance = midPoint.distanceTo(this->points[h]);
            }
        }
        cout<<"Distance = "<<maxDistance<<endl;
    }

    protected:
    vector<Point*> points;
};
