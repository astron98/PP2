#ifndef TRAJECTORY_H_INCLUDED
#define TRAJECTORY_H_INCLUDED
#include<bits/stdc++.h>
#include<cmath>

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
    float euclideanDistance(Points p1, Points p2){
        return sqrt(
            pow(p2.x-p1.x,2) + pow(p2.y-p1.y,2) + pow(p2.z-p1.z,2)
        );
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
};


#endif // TRAJECTORY_H_INCLUDED
