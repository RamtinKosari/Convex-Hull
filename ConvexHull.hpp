#ifndef CONVEXHULL_HPP
#define CONVEXHULL_HPP

#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;
using namespace this_thread;

extern int dotsMaxAmount;
extern int windowLength;
extern int windowWidth;
extern double half;
extern int distributionX;
extern int distributionY;
extern int dotRadius;
extern int dotBlue;
extern int dotGreen;
extern int dotRed;
extern int part;
extern int sign;

class Points {
    private:
        int x;
        int y;
        double theta;
        int hullPointX;
        int hullPointY;
    public:
        void setPosition(int, int, double);
        void setX(int);
        void setY(int);
        void setTheta(double);
        int accessX();
        int accessY();
        int accessTheta();
        void import2ConvexHull(int, int);
};

class ConvexHull : public Points {
    private:
        Points point[5000];
        cv::Mat preview;
        cv::Mat bar;
        vector <int> xCoord;
        vector <int> yCoord;
        vector <double> thetaStack;
        vector <int> xConvex;
        vector <int> yConvex;
        vector <double> thetaConvex;
        double degreeStack[5000];
        int amount;
        int originX;
        int originY;
        int originPlace;
        int selectedX;
        int selectedY;
    public:
        ConvexHull(int &);
        ConvexHull(string &, int &);
        void findLowestPoint();
        void calculateTheta();
        void sortbyTheta();
        void createConvexHull();
        long long int determinant(int, int, int, int, int, int);
        void resetStack();
        void progressBar(int, string);
};

#endif // CONVEXHULL_HPP