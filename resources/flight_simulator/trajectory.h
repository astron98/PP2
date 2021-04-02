#ifndef TRAJECTORY_H_INCLUDED
#define TRAJECTORY_H_INCLUDED
#include<bits/stdc++.h>

using namespace std;

class Points{
public:
    float x,y,z;
    Points(float x_1, float y_1, float z_1){

        x=x_1;
        y=y_1;
        z=z_1;

    }
};

class Trajectory::Points{
public:
    vector<Points> points;
    int numberOfPoints;
    Trajectory(vector<Points> &points_input){

        points = points_input;
        numberOfPoints = points.size();

    }
};


#endif // TRAJECTORY_H_INCLUDED
