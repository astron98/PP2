#ifndef TRAJECTORY_H_INCLUDED
#define TRAJECTORY_H_INCLUDED
#include<bits/stdc++.h>

using namespace std;

class Points{
public:
    float x,y,z;
    Points(){}
    Points(float x, float y, float z){

        this->x=x;
        this->y=y;
        this->z=z;

    }
};

class Trajectory{
public:
    vector<Points> points;
    int numberOfPoints;
    Points currentPoint;
    Points nextPoint;

    int cpi = 0;

    Trajectory(){}
    Trajectory(vector<Points> &points){

        this->points = points;
        this->numberOfPoints = this->points.size();

    }

    vector<float> findLine(Points a_point, Points b_point) {
        float a = (b_point.y - a_point.y)*(b_point.z-a_point.z);
        float b = (a_point.x - b_point.x)*(b_point.z-a_point.z);
        float c=  (a_point.x- b_point.x)*(b_point.y-a_point.y);
        float d = a*a_point.x + b*a_point.y+c*a_point.z;
        float da= -d/a;
        float db= -d/b;
        float dc= -d/c;

        vector<float> direction;
        direction.push_back(da);
        direction.push_back(db);
        direction.push_back(dc);
        return a1;

    }

};


#endif // TRAJECTORY_H_INCLUDED
