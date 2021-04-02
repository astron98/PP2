
#include<iostream>
using namespace std;

vector<float> findLine(float points[2][3]) {
   float a = (points[1][1] - points[0][1])*(points[1][2]-points[0][2]);
   float b = (points[0][0] - points[1][0])*(points[1][2]-points[0][2]);
   float c=  (points[0][0]- points[1][0])*(points[1][1]-points[0][1]);
   float d = a*points[0][0] + b*points[0][1]+c*points[0][2];
   float da= -d/a;
   float db= -d/b;
   float dc= -d/c;
   vector<float> a1;
   a1.push_back(da);
   a1.push_back(db);
   a1.push_back(dc);
   return a1;

 }
