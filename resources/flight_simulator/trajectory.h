#ifndef TRAJECTORY_H_INCLUDED
#define TRAJECTORY_H_INCLUDED
#include<bits/stdc++.h>
#include<cmath>

using namespace std;

class Points{
public:
    double x,y,z;
    Points(){}
    Points(double x, double y, double z){

        this->x=x;
        this->y=y;
        this->z=z;

    }
    double euclideanDistance(Points p1, Points p2){
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

    vector<double> directionVector(Points a_point, Points b_point) {
        double a = (b_point.y - a_point.y)*(b_point.z-a_point.z);
        double b = (a_point.x - b_point.x)*(b_point.z-a_point.z);
        double c=  (a_point.x- b_point.x)*(b_point.y-a_point.y);
        double d = a*a_point.x + b*a_point.y+c*a_point.z;
        double da= -d/a;
        double db= -d/b;
        double dc= -d/c;

        vector<double> direction;
        direction.push_back(da);
        direction.push_back(db);
        direction.push_back(dc);
        return direction;

    }

};


#endif // TRAJECTORY_H_INCLUDED
