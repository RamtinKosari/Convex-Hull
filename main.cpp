#include "ConvexHull.hpp"

int dotsMaxAmount = 4000;
int windowLength = 1000;
int windowWidth = 1000;
double half = 0.5;
int distributionX = windowLength * half;
int distributionY = windowWidth * half;
int dotRadius = 1;
int dotBlue = 159;
int dotGreen = 200;
int dotRed = 54;
int sign = 1;
int part = 1;

int main() {
    cout << "\033[H\033[2J\033[3J";
    cout << "\033[1;32mAvalible Modes \033[0m:" << endl;
    cout << "\033[1;36m1- \033[0mConvex Hull on Dots" << endl;
    cout << "\033[1;36m2- \033[0mConvex Hull on Image" << endl;
    cout << "\033[1;36m3- \033[0mConvex Hull on Live Camera" << endl;
    cout << endl;
    int choice;
    cout << "\033[1;33mChoice \033[0m: \033[1;32m"; cin >> choice;
    switch(choice) {
        case 1: {
            cout << "\033[H\033[2J\033[3J";
            int amount;
            cout << "\033[1;31mMaximum Amount of Dots is " << dotsMaxAmount << endl;
            cout << "\033[1;32mEnter Amount of Dots to Calculate its Convex Hull\033[0m : \033[1;37m"; cin >> amount;
            if (amount > dotsMaxAmount) {
                cout << "\033[1;37mMaximum Amount is \033[0m" << dotsMaxAmount << endl;
                amount = dotsMaxAmount;
            }
            cout << "\033[0m" << endl;
            ConvexHull onDots(amount);
            onDots.findLowestPoint();
            onDots.calculateTheta();
            onDots.sortbyTheta();
            onDots.createConvexHull();
            break;
        }
        case 2: {
            cout << "\033[H\033[2J\033[3J";
            string path;
            int scale;
            cout << "\033[1;32mEnter Path of the Image \033[0m: \033[1;37m"; cin >> path;
            cout << "\033[0m" << endl;
            cout << "\033[1;32mEnter Scale of the Image \033[0m: \033[1;37m"; cin >> scale;
            cout << "\033[0m" << endl;
            ConvexHull onImage(path, scale);
            onImage.sobel();
            break;
        }
        case 3: {

        }
        default: {
            cout << "\033[1;31mError : \033[1;33mThere is No Option\033[1;31m " << choice << "\033[1;33m.\033[0m" << endl;
        }
    }
    cv::waitKey(0);
    cv::destroyAllWindows();
    cout << "\033[1;37mProgram Terminated \033[1;32mSuccessfully \033[0m" << endl;
    return 0;
}
