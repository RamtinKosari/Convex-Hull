#include "ConvexHull.hpp"
typedef long long ll;
//-- Constructor to Create Dots on Window
ConvexHull::ConvexHull(int &inputAmount) {
    amount = inputAmount;
    preview = cv::Mat(windowLength, windowWidth, CV_8UC3, cv::Scalar(9, 1, 0));
    cout << "\033[1;37mGenerating Points...\033[0m" << endl;
    cv::imshow("output", preview);
    xCoord.resize(amount);
    yCoord.resize(amount);
    thetaStack.resize(amount);
    xConvex.resize(0);
    yConvex.resize(0);
    thetaConvex.resize(amount);
    int min = 0;
    int max;
    if (windowLength <= windowWidth) {
        max = windowLength - distributionX;
    } else if (windowLength > windowWidth) {
        max = windowWidth - distributionY;
    } else {
        max = windowLength + windowWidth;
    }
    random_device _random;
    default_random_engine eng(_random());
    uniform_int_distribution<int> distr(min, max);
    int amountOfGenerated;
    for (int i = 0; i < amount; i++) {
        amountOfGenerated = i + 1;
        if (cv::waitKey(1) == int('q')) {
            cout << "\033[1;37mProccess Has been \033[1;31mTerminated\033[1;37m" << endl;
            break;
        }
        int x = distr(eng);
        int y = distr(eng);
        int noise = distr(eng) * 1 / 3;
        sign *= -1;
        part++;
        if (part == 1) {
            x += noise * sign;
            y += noise * sign;
        } else if (part == 2) {
            x += noise * -sign;
            y += noise * sign;
        } else if (part == 3) {
            x += noise * -sign;
            y += noise * -sign;
        } else if (part == 4) {
            x += noise * sign;
            y += noise * -sign;
        }
        circle(preview, cv::Point(distributionX * half + x, distributionY * half + y), dotRadius, cv::Scalar(dotBlue, dotGreen, dotRed), -1, 8, 0);
        if (part == 4) {
            part = 1;
        }
        xCoord[i] = distributionX * half + x;
        yCoord[i] = distributionY * half + y;
        cv::imshow("output", preview);
        progressBar(i + 1, "Creating Dots");
    }
    cv::destroyWindow("Creating Dots");
    cout << "\033[1;37m" << amountOfGenerated << " Points Have been Generated \033[1;32mSuccessfully" << endl << endl;
}

//-- Constructor to Create Dots on Picture
ConvexHull::ConvexHull(string &inputPath, int &inputScale) {
    cout << "string" << endl;
}

//-- Method to Find Origin Point
void ConvexHull::findLowestPoint() {
    cout << "\033[1;37mFinding Lowest Point...\033[0m" << endl;
    int min = 0;
    int j;
    cv::Mat tmp;
    preview.copyTo(tmp);
    imshow("output", tmp);
    for (int i = 0; i < amount; i++) {
        circle(tmp, cv::Point(xCoord[i], yCoord[i]), dotRadius, cv::Scalar(0, 255, 255), -1, 8, 0);
        if (yCoord[i] > min) {
            min = yCoord[i];
            j = i;
            cv::circle(tmp, cv::Point(xCoord[j], yCoord[j]), dotRadius * 2, cv::Scalar(0, 0, 255), -1, 8, 0);
            cv::line(tmp, cv::Point(xCoord[j] - 5, yCoord[j]), cv::Point(xCoord[j] - 20, yCoord[j]), cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::line(tmp, cv::Point(xCoord[j] + 5, yCoord[j]), cv::Point(xCoord[j] + 20, yCoord[j]), cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::line(tmp, cv::Point(xCoord[j], yCoord[j] + 5), cv::Point(xCoord[j], yCoord[j] + 20), cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::line(tmp, cv::Point(xCoord[j], yCoord[j] - 5), cv::Point(xCoord[j], yCoord[j] - 20), cv::Scalar(255, 255, 255), 1, 8, 0);
        }
        cv::waitKey(1);
        cv::imshow("output", tmp);
        progressBar(i + 1, "Finding Lowest Point");
    }
    cv::destroyWindow("Finding Lowest Point");
    circle(tmp, cv::Point(xCoord[j], yCoord[j]), 2, cv::Scalar(0, 255, 0), -1, 8, 0);
    circle(preview, cv::Point(xCoord[j], yCoord[j]), 2, cv::Scalar(0, 255, 0), -1, 8, 0);
    cv::line(tmp, cv::Point(xCoord[j] - 5, yCoord[j]), cv::Point(xCoord[j] - 20, yCoord[j]), cv::Scalar(0, 255, 0), 1, 8, 0);
    cv::line(tmp, cv::Point(xCoord[j] + 5, yCoord[j]), cv::Point(xCoord[j] + 20, yCoord[j]), cv::Scalar(0, 255, 0), 1, 8, 0);
    cv::line(preview, cv::Point(xCoord[j] - 20, yCoord[j] + 18), cv::Point(xCoord[j] + 20, yCoord[j] + 18), cv::Scalar(200, 200, 200), 1, 8, 0);
    cv::line(preview, cv::Point(xCoord[j] - 10, yCoord[j] + 13), cv::Point(xCoord[j] + 10, yCoord[j] + 13), cv::Scalar(200, 200, 200), 1, 8, 0);
    cv::line(tmp, cv::Point(xCoord[j], yCoord[j] + 5), cv::Point(xCoord[j], yCoord[j] + 20), cv::Scalar(0, 255, 0), 1, 8, 0);
    cv::line(tmp, cv::Point(xCoord[j], yCoord[j] - 5), cv::Point(xCoord[j], yCoord[j] - 20), cv::Scalar(0, 255, 0), 1, 8, 0);
    cv::imshow("output", tmp);
    cv::waitKey(1);
    originX = xCoord[j];
    originY = yCoord[j];
    originPlace = j;
    cout << "\033[1;37mLowest Point Has been Found at (" << xCoord[j] << ", " << yCoord[j] << ") \033[1;32mSuccessfully\033[0m" << endl << endl;
}

//-- Calculate Dots Angle from Horizental Axis of Origin Dot
void ConvexHull::calculateTheta() {
    cout << "\033[1;37mProccessing Positions to Start Sorting Theta...\033[0m" << endl;
    cv::Mat tmp;
    preview.copyTo(tmp);
    cv::imshow("output", tmp);
    cv::Mat tmp2;
    for (int i = 0; i < amount; i++) {
        thetaStack[i] = atan2(originY - yCoord[i], xCoord[i] - originX);
        degreeStack[i] = thetaStack[i] * 180 / M_PI;
        point[i].setPosition(xCoord[i], yCoord[i], degreeStack[i]);
        if (thetaStack[i] != 0) {
            if (i % 3 == 0) {
                tmp.copyTo(tmp2);
            }
            cv::line(tmp2, cv::Point(originX, originY), cv::Point(originX + sqrt(pow((xCoord[i] - originX), 2) + pow((originY - yCoord[i]), 2)) * cos(2 * M_PI - thetaStack[i]), originY + sqrt(pow((xCoord[i] - originX), 2) + pow((originY - yCoord[i]), 2)) * sin(2 * M_PI - thetaStack[i])),cv:: Scalar(255, 255, 255), 1, 8, 0);
            cv::circle(tmp2, cv::Point(xCoord[i], yCoord[i]), 1, cv::Scalar(255, 0, 255), -1, 8, 0);
            cv::circle(tmp, cv::Point(xCoord[i], yCoord[i]), 1, cv::Scalar(255, 0, 0), -1, 8, 0);
            if (thetaStack[i] == thetaStack[i - 1]) {
                cout << "/033[1;35mErr\033[0m" << endl;
            }
            cv::imshow("output", tmp2);
        }
        cv::waitKey(1);
        progressBar(i + 1, "Calculating Dots Angle From Origin Dot");
    }
    cv::destroyWindow("Calculating Dots Angle From Origin Dot");
    cv::imshow("output", tmp);
    cout << "\033[1;37mProcess Done \033[1;32mSuccessfully\033[0m" << endl << endl;
}

//-- Sort Dots by Their Theta
void ConvexHull::sortbyTheta() {
    cout << "\033[1;37mStart Sorting Points Around Origin Point by Their Theta...\033[0m" << endl;
    cv::Mat tmp;
    cv::Mat tmp2;
    preview.copyTo(tmp);
    cv::imshow("output", tmp);
    for (int i = 0; i < amount; i++) {
        progressBar(i + 1, "Sorting Dots by THeir Theta");
        tmp.copyTo(tmp2);
        if (i < amount) {
            cv::line(tmp, cv::Point(xCoord[i], yCoord[i]), cv::Point(xCoord[i + 1], yCoord[i + 1]), cv::Scalar(0, 100, 0), 1, 8, 0);
            if (i < amount - 1) {
                cv::line(tmp, cv::Point(xCoord[i + 1], yCoord[i + 1]), cv::Point(xCoord[i + 2], yCoord[i + 2]), cv::Scalar(0, 0, 255), 1, 8, 0);
            }
        }
        for (int j = 0; j < amount - i - 1; j++) {
            if (point[j].accessTheta() > point[j + 1].accessTheta()) {
                int tmpTheta = point[j + 1].accessTheta();
                point[j + 1].setTheta(point[j].accessTheta());
                point[j].setTheta(tmpTheta);
                int tmpX = point[j + 1].accessX();
                point[j + 1].setX(point[j].accessX());
                point[j].setX(tmpX);
                int tmpY = point[j + 1].accessY();
                point[j + 1].setY(point[j].accessY());
                point[j].setY(tmpY);
                cv::line(tmp2, cv::Point(xCoord[j], yCoord[j]), cv::Point(xCoord[j + 1], yCoord[j + 1]), cv::Scalar(50, 50, 50), 1, 8, 0);
                cv::circle(tmp2, cv::Point(xCoord[j], yCoord[j]), 1, cv::Scalar(0, 255, 0), -1, 8, 0);
                cv::circle(tmp2, cv::Point(xCoord[j + 1], yCoord[j + 1]), 1, cv::Scalar(0, 0, 255), -1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j] - 5, yCoord[j]), cv::Point(xCoord[j] - 7, yCoord[j]), cv::Scalar(255, 255, 255), 1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j] + 5, yCoord[j]), cv::Point(xCoord[j] + 7, yCoord[j]), cv::Scalar(255, 255, 255), 1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j], yCoord[j] + 5), cv::Point(xCoord[j], yCoord[j] + 7), cv::Scalar(255, 255, 255), 1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j], yCoord[j] - 5), cv::Point(xCoord[j], yCoord[j] - 7), cv::Scalar(255, 255, 255), 1, 8, 0);
                // cv::imshow("output", tmp2);
                // cv::waitKey(1);
            } else if (point[j].accessTheta() == point[j + 1].accessTheta()) {

            }
        }
        cv::line(tmp, cv::Point(xCoord[i] - 5, yCoord[i]), cv::Point(xCoord[i] - 10, yCoord[i]), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::line(tmp, cv::Point(xCoord[i] + 5, yCoord[i]), cv::Point(xCoord[i] + 10, yCoord[i]), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::line(tmp, cv::Point(xCoord[i], yCoord[i] + 5), cv::Point(xCoord[i], yCoord[i] + 10), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::line(tmp, cv::Point(xCoord[i], yCoord[i] - 5), cv::Point(xCoord[i], yCoord[i] - 10), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::circle(tmp, cv::Point(xCoord[i], yCoord[i]), 1, cv::Scalar(255, 255, 255), -1, 8, 0);
    }
    //-- Sort Same Theta in Stack According to Distance from Origin Point
    // for (int i = 0; i < amount; i++) {

    // }
    cv::destroyWindow("Sorting Dots by THeir Theta");
    preview.copyTo(tmp);
    int colorValue;
    for (int i = 0; i < amount; i++) {
        colorValue = 200 * i / amount;
        cv::circle(tmp, cv::Point(point[i].accessX(), point[i].accessY()), 1, cv::Scalar(0, 55 + colorValue, 0), -1, 8, 0);
        if (i < amount - 1) {   
            cv::line(tmp, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(60, 60, 60), 1, 8, 0);
        }
        if (i < amount - 2) {
            cv::line(tmp, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(60, 60, 60), 1, 8, 0);
        }
        cv::imshow("output", tmp);
        cv::waitKey(1);
        progressBar(i + 1, "Processing");
        sleep_for(milliseconds(2));
    }
    cv::destroyWindow("Processing");
    cv::destroyWindow("Preparing to Affect Convex Hull");
    cout << "\033[1;37mSorting Done \033[1;32mSuccessfully\033[0m" << endl << endl;
}

//-- Creates Convex Hull of Generated Dots
void ConvexHull::createConvexHull() {
    cv::Mat tmp;
    cv::Mat tmp2;
    preview.copyTo(tmp);
    preview.copyTo(tmp2);
    cv::imshow("output", tmp2);
    cout << "\033[1;37mGenerating Convex Hull...\033[0m" << endl;
    for (int i = 1; i < amount; i++) {
        cv::putText(tmp2, to_string(i) + ": " + to_string(point[i].accessX()) + "/" + to_string(point[i].accessY()), cv::Point(point[i].accessX() + 8, point[i].accessY() + 14), cv::FONT_HERSHEY_TRIPLEX, 0.3, cv::Scalar(255, 255, 255));
        cv::putText(tmp, to_string(i), cv::Point(point[i].accessX() + 8, point[i].accessY() + 14), cv::FONT_HERSHEY_TRIPLEX, 0.3, cv::Scalar(255, 255, 255));
        cv::imshow("output", tmp2);
        cv::waitKey(1);
    }
    tmp.copyTo(tmp2);
    //-- Config Section
    double signX;
    double signY;
    double txtPosX;
    double txtPosY;
    double iterPosX;
    double iterPosY;
    int counter = 2;
    //-- Add First 3 Points into Stack of Convex Points
    xConvex.push_back(point[0].accessX());
    xConvex.push_back(point[1].accessX());
    xConvex.push_back(point[2].accessX());
    yConvex.push_back(point[0].accessY());
    yConvex.push_back(point[1].accessY());
    yConvex.push_back(point[2].accessY());
    //- Loop Section
    for (int i = 3; i < amount; i++) {
        tmp.copyTo(tmp2);
        //--------------------
        //--| Draw Section |--
        //--------------------
        //- First Last Point
        cv::putText(tmp2, "First Last", cv::Point(point[i - 1].accessX() + 17, point[i - 1].accessY() + 3.5), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 170, 150));
        cv::line(tmp2, cv::Point(point[i - 1].accessX() + 2, point[i - 1].accessY() + 2), cv::Point(point[i - 1].accessX() + 12, point[i - 1].accessY() + 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i - 1].accessX() - 2, point[i - 1].accessY() - 2), cv::Point(point[i - 1].accessX() - 12, point[i - 1].accessY() - 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i - 1].accessX() - 2, point[i - 1].accessY() + 2), cv::Point(point[i - 1].accessX() - 12, point[i - 1].accessY() + 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i - 1].accessX() + 2, point[i - 1].accessY() - 2), cv::Point(point[i - 1].accessX() + 12, point[i - 1].accessY() - 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        //- Second Last Point
        cv::putText(tmp2, "Second Last", cv::Point(point[i - 2].accessX() + 17, point[i - 2].accessY() + 3.5), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 170, 150));
        cv::line(tmp2, cv::Point(point[i - 2].accessX() + 2, point[i - 2].accessY() + 2), cv::Point(point[i - 2].accessX() + 12, point[i - 2].accessY() + 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i - 2].accessX() - 2, point[i - 2].accessY() - 2), cv::Point(point[i - 2].accessX() - 12, point[i - 2].accessY() - 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i - 2].accessX() - 2, point[i - 2].accessY() + 2), cv::Point(point[i - 2].accessX() - 12, point[i - 2].accessY() + 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i - 2].accessX() + 2, point[i - 2].accessY() - 2), cv::Point(point[i - 2].accessX() + 12, point[i - 2].accessY() - 7), cv::Scalar(0, 100, 255), 1, 8, 0);
        //- Iterated Point
        if (point[i].accessX() >= windowWidth / 2) {
            signX = -1;
            txtPosX = 1.45;
            if (i > 999) {
                iterPosX = 2.2;
            } else if(i > 99) {
                iterPosX = 2;
            } else if (i > 9) {
                iterPosX = 1.8;
            } else {
                iterPosX = 1.6;
            }
        } 
        if (point[i].accessX() < windowWidth / 2) {
            signX = 1;
            txtPosX = 1.05;
            iterPosX = 1;
        } 
        if (point[i].accessY() >= windowLength / 2) {
            signY = -1;
            txtPosY = 0.85;
            iterPosY = 0.6;
        } 
        if (point[i].accessY() < windowLength / 2) {
            signY = 1;
            txtPosY = 1;
            iterPosY = 1;
        } 
        cv::circle(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), 10, cv::Scalar(255, 200, 12), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i].accessX() + 5, point[i].accessY() + 5), cv::Point(point[i].accessX() + 12, point[i].accessY() + 12), cv::Scalar(255, 255, 255), 1, 16, 0);
        cv::line(tmp2, cv::Point(point[i].accessX() - 5, point[i].accessY() - 5), cv::Point(point[i].accessX() - 12, point[i].accessY() - 12), cv::Scalar(255, 255, 255), 1, 16, 0);
        cv::line(tmp2, cv::Point(point[i].accessX() - 5, point[i].accessY() + 5), cv::Point(point[i].accessX() - 12, point[i].accessY() + 12), cv::Scalar(255, 255, 255), 1, 16, 0);
        cv::line(tmp2, cv::Point(point[i].accessX() + 5, point[i].accessY() - 5), cv::Point(point[i].accessX() + 12, point[i].accessY() - 12), cv::Scalar(255, 255, 255), 1, 16, 0);
        cv::line(tmp2, cv::Point(point[i].accessX() + signX * 16, point[i].accessY() + signY * 16), cv::Point(point[i].accessX() + signX * 40, point[i].accessY() + signY * 16), cv::Scalar(255, 255, 255), 1, 16, 0);
        cv::line(tmp2, cv::Point(point[i].accessX() + signX * 40, point[i].accessY() + signY * 21), cv::Point(point[i].accessX() + signX * 40, point[i].accessY() + signY * 11), cv::Scalar(255, 255, 255), 1, 16, 0);
        cv::putText(tmp2, "I : " + to_string(i), cv::Point(point[i].accessX() + signX * 46 * iterPosX, point[i].accessY() + signY * 19 * iterPosY), cv::FONT_HERSHEY_DUPLEX, 0.4, cv::Scalar(235, 255, 110));
        cv::line(tmp2, cv::Point(point[i].accessX() + signX * 12, point[i].accessY() + signY * 12), cv::Point(point[i].accessX() + signX * 40, point[i].accessY() + signY * 40), cv::Scalar(255, 255, 255), 1, 16, 0);
        cv::rectangle(tmp2, cv::Point(point[i].accessX() + signX * 110, point[i].accessY() + signY * 20), cv::Point(point[i].accessX() + signX * 190, point[i].accessY() + signY * 60), cv::Scalar(26, 13, 2), -1, 8, 0);
        cv::rectangle(tmp2, cv::Point(point[i].accessX() + signX * 110, point[i].accessY() + signY * 20), cv::Point(point[i].accessX() + signX * 190, point[i].accessY() + signY * 60), cv::Scalar(255, 255, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i].accessX() + signX * 40, point[i].accessY() + signY * 40), cv::Point(point[i].accessX() + signX * 110, point[i].accessY() + signY * 40), cv::Scalar(255, 255, 255), 1, 8, 0);
        cv::putText(tmp2, "X : " + to_string(point[i].accessX()), cv::Point(point[i].accessX() + signX * 120 * txtPosX, point[i].accessY() + signY * 34 * txtPosY), cv::FONT_HERSHEY_DUPLEX, 0.4, cv::Scalar(255, 220, 120));
        cv::putText(tmp2, "Y : " + to_string(point[i].accessY()), cv::Point(point[i].accessX() + signX * 120 * txtPosX, point[i].accessY() + signY * 52 * txtPosY), cv::FONT_HERSHEY_DUPLEX, 0.4, cv::Scalar(255, 220, 120));
        //-- Determinant
        // int det = determinant(point[i].accessX(), point[i].accessY(), xConvex[counter], yConvex[counter], xConvex[counter - 1], yConvex[counter - 1]);
        int det = determinant(point[i].accessX(), point[i].accessY(), point[i - 1].accessX(), point[i - 1].accessY(), point[i - 2].accessX(), point[i - 2].accessY());
        cout << "det " << det << endl;
        cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i - 1].accessX(), point[i - 1].accessY()), cv::Scalar(0, 140, 200), 1, 8, 0);
        cv::line(tmp2, cv::Point(point[i - 2].accessX(), point[i - 2].accessY()), cv::Point(point[i - 1].accessX(), point[i - 1].accessY()), cv::Scalar(0, 140, 200), 1, 8, 0);
        // cv::line(tmp2, cv::Point(xConvex[counter - 1], yConvex[counter - 1]), cv::Point(xConvex[counter], yConvex[counter]), cv::Scalar(30, 160, 150), 1, 8, 0);
        // cv::line(tmp2, cv::Point(xConvex[counter - 1], yConvex[counter - 1]), cv::Point(xConvex[counter - 2], yConvex[counter - 2]), cv::Scalar(255, 255, 255), 1, 8, 0);
        // 210, 170, 150
        //-- Push to or Pop from Convex Stack
        // while (true) {
        //     if (determinant(point[i].accessX(), point[i].accessY(), xConvex[counter], yConvex[counter], xConvex[counter - 1], yConvex[counter - 1]) > 0) {
        //         // cv::line(tmp2, cv::Point(xConvex[counter], yConvex[counter]), cv::Point(point[i].accessX(), point[i].accessY()), cv::Scalar(0, 255, 0), 1, 8, 0);
        //         xConvex.push_back(point[i].accessX());
        //         yConvex.push_back(point[i].accessY());
        //         cout << "added point " << i << " to convex" << endl;
        //         counter++;
        //         break;
        //     } else if (determinant(point[i].accessX(), point[i].accessY(), xConvex[counter], yConvex[counter], xConvex[counter - 1], yConvex[counter - 1]) < 0) {
        //         xConvex.pop_back();
        //         yConvex.pop_back();
        //         cout << "removed point " << i << " to convex" << endl;
        //         xConvex.push_back(point[i].accessX());
        //         xConvex.push_back(point[i].accessY());
        //         break;
        //     } else {
        //         break;
        //     }
        // }
        // while (true) {
        //     det = determinant(point[i].accessX(), point[i].accessY(), xConvex[counter], yConvex[counter], xConvex[counter - 1], yConvex[counter - 1]);
        //     if (det < 0 )
        // }
        // xConvex.push_back(point[i].accessX());
        // yConvex.push_back(point[i].accessY());
        // counter++;
        // while (determinant(point[i].accessX(), point[i].accessY(), xConvex[counter], yConvex[counter], xConvex[counter - 1], yConvex[counter - 1]) < 0) {
        //     xConvex.pop_back();
        //     yConvex.pop_back();
        // }
        while (true) {
            cv::line(tmp2, cv::Point(xConvex[counter], yConvex[counter]), cv::Point(xConvex[counter - 1], yConvex[counter - 1]), cv::Scalar(255, 255, 0), 1, 8, 0);
            cv::line(tmp2, cv::Point(xConvex[counter - 2], yConvex[counter - 2]), cv::Point(xConvex[counter - 1], yConvex[counter - 1]), cv::Scalar(255, 255, 0), 1, 8, 0);
            if (determinant(point[i].accessX(), point[i].accessY(), xConvex[counter], yConvex[counter], xConvex[counter - 1], yConvex[counter - 1]) > 0) {
                cv::line(tmp2, cv::Point(xConvex[counter], yConvex[counter]), cv::Point(point[i].accessX(), point[i].accessY()), cv::Scalar(0, 255, 0), 1, 8, 0);
                xConvex.push_back(point[i].accessX());
                yConvex.push_back(point[i].accessY());
                counter++;
                break;
            } else if (determinant(point[i].accessX(), point[i].accessY(), xConvex[counter], yConvex[counter], xConvex[counter - 1], yConvex[counter - 1]) < 0) {
                cv::line(tmp2, cv::Point(xConvex[counter], yConvex[counter]), cv::Point(point[i].accessX(), point[i].accessY()), cv::Scalar(0, 0, 255), 1, 8, 0);
                xConvex.pop_back();
                yConvex.pop_back();
                counter--;
            } else if (point[i].accessTheta() == point[i - 1].accessTheta()) {
                cv::rectangle(preview, cv::Point(point[i].accessX() + 5, point[i].accessY() + 5), cv::Point(point[i].accessX() - 5, point[i].accessY() - 5), cv::Scalar(0, 150, 190), 1, 8, 0);
                cv::line(preview, cv::Point(point[i].accessX() + 5, point[i].accessY()), cv::Point(point[i].accessX() + 12, point[i].accessY()), cv::Scalar(0, 255, 255), 1, 16, 0);
                cv::line(preview, cv::Point(point[i].accessX() - 5, point[i].accessY()), cv::Point(point[i].accessX() - 12, point[i].accessY()), cv::Scalar(0, 255, 255), 1, 16, 0);
                cv::line(preview, cv::Point(point[i].accessX(), point[i].accessY() + 5), cv::Point(point[i].accessX(), point[i].accessY() + 12), cv::Scalar(0, 255, 255), 1, 16, 0);
                cv::line(preview, cv::Point(point[i].accessX(), point[i].accessY() - 5), cv::Point(point[i].accessX(), point[i].accessY() - 12), cv::Scalar(0, 255, 255), 1, 16, 0);
                double height = 22 * sin(M_PI / 3);
                cv::line(tmp2, cv::Point(point[i].accessX() - 11, point[i].accessY() - height * 0.5), cv::Point(point[i].accessX() + 11, point[i].accessY() - height * 0.5), cv::Scalar(0, 255, 255), 1, 16, 0);
                cv::line(tmp2, cv::Point(point[i].accessX() - 11, point[i].accessY() - height * 0.5), cv::Point(point[i].accessX(), point[i].accessY() + height * 0.5), cv::Scalar(0, 255, 255), 1, 16, 0);
                cv::line(tmp2, cv::Point(point[i].accessX() + 11, point[i].accessY() - height * 0.5), cv::Point(point[i].accessX(), point[i].accessY() + height * 0.5), cv::Scalar(0, 255, 255), 1, 16, 0);
                cv::imshow("output", tmp2);
                cv::waitKey(1);
                cout << "err" << endl;
                // int dist1 = sqrt();
                // int dist2;
                // counter++;
                xConvex.push_back(point[i].accessX());
                yConvex.push_back(point[i].accessY());
            }
            cv::waitKey(1);
        }
        imshow("output", tmp2);
        cv::waitKey(1);
    }
    for (int i = 0; i < xConvex.size(); i++) {
        cout << "\033[1;33m" << xConvex[i] << " - " << yConvex[i] << "\033[0m" << endl;
    }
    cv::waitKey(1);
    //-- Showing Result
    preview.copyTo(tmp2);
    for (int i = 1; i < xConvex.size(); i++) {
        cv::line(tmp2, cv::Point(xConvex[i], yConvex[i]), cv::Point(xConvex[i - 1], yConvex[i - 1]), cv::Scalar(0, 100, 255), 1, 8, 0);
        cv::imshow("output", tmp2);
        cv::waitKey(1);
    }
    cv::line(tmp2, cv::Point(xConvex[0], yConvex[0]), cv::Point(xConvex[xConvex.size() - 1], yConvex[xConvex.size() - 1]), cv::Scalar(0, 100, 255), 1, 8, 0);
    for (int i = 0; i < xConvex.size(); i++) {
        cv::circle(tmp2, cv::Point(xConvex[i], yConvex[i]), 2, cv::Scalar(0, 0, 255), -1, 8, 0);
        cv::imshow("output", tmp2);
        cv::waitKey(1);
    }
    double height = 22 * sin(M_PI / 3);
    for (int i = 0; i < xConvex.size(); i++) {
        cv::line(tmp2, cv::Point(xConvex[i] - 11, yConvex[i] + height * 0.5 - 2), cv::Point(xConvex[i] + 11, yConvex[i] + height * 0.5 - 2), cv::Scalar(0, 255, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(xConvex[i] - 11, yConvex[i] + height * 0.5 - 2), cv::Point(xConvex[i], yConvex[i] - height * 0.5 - 2), cv::Scalar(0, 255, 255), 1, 8, 0);
        cv::line(tmp2, cv::Point(xConvex[i] + 11, yConvex[i] + height * 0.5 - 2), cv::Point(xConvex[i], yConvex[i] - height * 0.5 - 2), cv::Scalar(0, 255, 255), 1, 8, 0);
        cv::imshow("output", tmp2);
        cv::waitKey(1);
    }

    // cv::waitKey(0);
    // int counter = 1;
    // xConvex.push_back(point[0].accessX());
    // yConvex.push_back(point[0].accessY());
    // xConvex.push_back(point[1].accessX());
    // yConvex.push_back(point[1].accessY());
    // xConvex.push_back(point[2].accessX());
    // yConvex.push_back(point[2].accessY());
    
    // for (int i = 0; i < amount - 2; i++) {
    //     tmp.copyTo(tmp2);
    //     //- Point First
        
    //     //- Point After First
    //     cv::putText(tmp2, "Second", cv::Point(point[i + 2].accessX() + 10, point[i + 2].accessY() + 16), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(190, 150, 130));
    //     cv::line(tmp2, cv::Point(point[i + 2].accessX() + 5, point[i + 2].accessY()), cv::Point(point[i + 2].accessX() + 10, point[i + 2].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
    //     cv::line(tmp2, cv::Point(point[i + 2].accessX() - 5, point[i + 2].accessY()), cv::Point(point[i + 2].accessX() - 10, point[i + 2].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
    //     cv::line(tmp2, cv::Point(point[i + 2].accessX(), point[i + 2].accessY() + 5), cv::Point(point[i + 2].accessX(), point[i + 2].accessY() + 10), cv::Scalar(0, 100, 255), 1, 8,+0);
    //     cv::line(tmp2, cv::Point(point[i + 2].accessX(), point[i + 2].accessY() - 5), cv::Point(point[i + 2].accessX(), point[i + 2].accessY() - 10), cv::Scalar(0, 100, 255), 1, 8,+0);

    //     // cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(0, 255, 255), 1, 8, 0);
    //     // cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(0, 255, 255), 1, 8, 0);
    //     cv::arrowedLine(tmp2, cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(255, 255, 0), 2, 8, 0);
    //     //-- Right Turn or Left Turn Section
    //     int det;
    //     if (i == 0) {
    //         det = determinant(point[i + 1].accessX(), point[i + 1].accessY(), point[i + 2].accessX(), point[i + 2].accessY(), originX, originY);
    //     } else {
    //         det = determinant(point[i + 1].accessX(), point[i + 1].accessY(), point[i + 2].accessX(), point[i + 2].accessY(), selectedX, selectedY);
    //     }
    //     cout << "\033[1;36mDelta of Gradients in Turn " << i << " :\033[0m " << det << endl;
    //     if (det < 0) {
    //         cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(255, 255, 0), 1, 8, 0);
    //         cv::line(tmp, cv::Point(selectedX, selectedY), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(255, 255, 255), 1, 8, 0);
    //         xConvex.push_back(point[i].accessX());
    //         yConvex.push_back(point[i].accessY());
    //         cv::line(tmp, cv::Point(xConvex[i], yConvex[i]), cv::Point(xConvex[i + 1], yConvex[i + 1]), cv::Scalar(255, 255, 255), 1, 8, 0);
    //     } else if (det > 0) {
    //         while(true) {
    //             if (determinant(point[i - counter + 1].accessX(), point[i - counter + 1].accessY(), point[i + 2].accessX(), point[i + 2].accessY(), point[i - counter].accessX(), point[i - counter].accessY())) {
    //                 cv::line(tmp2, cv::Point(xConvex[i], yConvex[i]), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(0, 255, 0), 1, 8, 0);
    //                 cv::line(tmp, cv::Point(selectedX, selectedY), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(255, 255, 255), 1, 8, 0);
    //                 selectedX = point[i - counter].accessX();
    //                 selectedY = point[i - counter].accessY();
    //                 counter = 1;
    //                 break;
    //             } else {
    //                 cv::line(tmp2, cv::Point(xConvex[i], yConvex[i]), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(0, 0, 255), 1, 8, 0);
    //                 xConvex.pop_back();
    //                 yConvex.pop_back();
    //             }
    //             xConvex.push_back(point[i - counter].accessX());
    //             yConvex.push_back(point[i - counter].accessY());
    //             counter++;
    //             cv::imshow("output", tmp2);
    //             cv::waitKey(1);
    //         }
            // cv::line(tmp, cv::Point(xConvex[i], yConvex[i]), cv::Point(xConvex[i - 1], yConvex[i - 1]), cv::Scalar(255, 255, 255), 1, 8, 0);
            // cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i - 1].accessX(), point[i - 1].accessY()), cv::Scalar(0, 0, 255), 1, 8, 0);
            // cv::line(tmp, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i - 2].accessX(), point[i - 2].accessY()), cv::Scalar(255, 255, 255), 1, 8, 0);
        // } else if (det == 0) {
            // int distFirst = sqrt(pow(point[i].accessX() - point[i + 1].accessX(), 2) + pow(point[i].accessY() - point[i + 1].accessY(), 2));
            // int distSecond = sqrt(pow(point[i].accessX() - point[i + 2].accessX(), 2) + pow(point[i].accessY() - point[i + 2].accessY(), 2));
            // if (distSecond > distFirst) {
            //     xConvex.push_back(point[i + 2].accessX());
            //     yConvex.push_back(point[i + 2].accessY());
            // } else if (distSecond )
        // } else {

        // }
        // cv::imshow("output", tmp2);
        // cv::waitKey(0);
    // }
    // tmp.copyTo(tmp2);
    // for (int i = 0; i < amount; i++) {
    //     cv::line(tmp2, cv::Point(xConvex[i], yConvex[i]), cv::Point(xConvex[i - 1], yConvex[i - 1]), cv::Scalar(255, 50, 0), 1, 8, 0);
    //     cv::imshow("output", tmp2);
    //     cv::waitKey(1);
    // }
    cv::destroyWindow("Creating Convex Hull");
    cout << "\033[1;37mConvex Hull Has Been Generated \033[1;32mSuccessfully\033[0m" << endl;
}

//-- Set to Scae Position of Dots
void Points::setPosition(int inputX, int inputY, double inputTheta) {
    x = inputX;
    y = inputY;
    theta = inputTheta;
}

//-- Set X
void Points::setX(int input) {
    x = input;
}

//-- Set Y
void Points::setY(int input) {
    y = input;
}

//-- Set Theta
void Points::setTheta(double input) {
    theta = input;
}

//-- Accessor to X
int Points::accessX() {
    return x;
}

//-- Accessor to Y
int Points::accessY() {
    return y;
}

//-- Accessor to Theta
int Points::accessTheta() {
    return theta;
}

void Points::import2ConvexHull(int inputX, int inputY) {
    hullPointX = inputX;
    hullPointY = inputY;
}

//-- Determinant Calculator
long long int ConvexHull::determinant(int xSelected, int ySelected ,int xCHLast, int yCHLast, int xCHBeforeLast, int yCHBeforeLast) {
    // int aX = xCHLast - xCHBeforeLast;
    // int aY = yCHLast - yCHBeforeLast;
    // int bX = xSelected - xCHLast;
    // int bY = ySelected - yCHLast;
    //-----
    int aX = xSelected - xCHLast;
    int aY = ySelected - yCHLast;
    int bX = xCHLast - xCHBeforeLast;
    int bY = yCHLast - yCHBeforeLast;
    //-----
    // int aX = xCHLast - xSelected;
    // int aY = yCHLast - ySelected;
    // int bX = xCHBeforeLast - xSelected;
    // int bY = yCHBeforeLast - ySelected;
    //-----
    // int v1x = xCHLast - xCHBeforeLast;
    // int v1y = yCHBeforeLast - yCHLast;
    // int v2x = xSelected - xCHLast;
    // int v2y = yCHLast - ySelected;
    //-----
    // int aX = xSelected - xCHLast;
    // int aY = yCHLast - ySelected;
    // int bX = xCHLast - xCHBeforeLast;
    // int bY = yCHBeforeLast - yCHLast;
    //-----
    long long int output = aX * bY - aY * bX;
    // int output = aX * bY - aY * bX;
    // int output = aY * bX - aX * bY;
    // int output = v1x * v2y - v1y * v2x;
    // int output = v1y * v2x - v1x * v2y;
    //-----
    return output;
}

void ConvexHull::progressBar(int progressValue, string name) {
    int length = windowLength * 0.5;
    int width = windowWidth * 0.1;
    int pCorner1Length = length * 0.1;
    int pCorner1Width = width * 0.6;
    int pCorner2Length = length * 0.9;
    int pCorner2Width = width * 0.8;
    int value = (pCorner1Length - pCorner2Length) * progressValue / amount;
    bar = cv::Mat(width, length, CV_8UC3, cv::Scalar(15, 4, 0));
    if ((-value) * 100 / (pCorner2Length - pCorner1Length) < 100) {
        cv::putText(bar, to_string((-value) * 100 / (pCorner2Length - pCorner1Length)) + "% Completed...", cv::Point(length * 0.1, width * 0.4), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(200, 180, 0));
    } else if ((pCorner1Length - value) * 100 / (pCorner2Length - pCorner1Length) >= 100) {
        cv::putText(bar, "Done !", cv::Point(length * 0.1, width * 0.3), cv::FONT_HERSHEY_TRIPLEX, 0.4, cv::Scalar(40, 255, 20));
    } else {
        cv::putText(bar, "Error", cv::Point(length * 0.1, width * 0.3), cv::FONT_HERSHEY_TRIPLEX, 0.4, cv::Scalar(0, 0, 255));
    }
    cv::rectangle(bar, cv::Point(pCorner1Length, pCorner1Width), cv::Point(pCorner2Length, pCorner2Width), cv::Scalar(45, 17, 0), -1, 8, 0);
    cv::rectangle(bar, cv::Point(pCorner1Length, pCorner1Width), cv::Point(pCorner1Length - value, pCorner2Width), cv::Scalar(180, 190, 0), -1, 8, 0);
    imshow(name, bar);
    cv::waitKey(1);
}