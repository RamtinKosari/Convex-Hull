#include "ConvexHull.hpp"

//-- Constructor to Create Dots on Window
ConvexHull::ConvexHull(int &inputAmount) {
    isMissed = 1;
    amount = inputAmount;
    preview = cv::Mat(windowLength, windowWidth, CV_8UC3, cv::Scalar(9, 1, 0));
    cout << "\033[1;37mGenerating Points...\033[0m" << endl;
    cv::imshow("output", preview);
    xCoord.resize(amount);
    yCoord.resize(amount);
    thetaStack.resize(amount);
    xConvex.resize(amount);
    yConvex.resize(amount);
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
    cv::line(preview, cv::Point(xCoord[j] - 20, yCoord[j] + 10), cv::Point(xCoord[j] + 20, yCoord[j] + 10), cv::Scalar(200, 200, 200), 1, 8, 0);
    cv::line(preview, cv::Point(xCoord[j] - 10, yCoord[j] + 18), cv::Point(xCoord[j] + 10, yCoord[j] + 18), cv::Scalar(200, 200, 200), 1, 8, 0);
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
    cout << "\033[1;37mStart Sorting Points Around Origin Point by Their Theta...\033[0m" << endl;
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
                int tmpXStack = posXStack[j + 1];
                posXStack[j + 1] = posXStack[j];
                posXStack[j] = tmpXStack;
                int tmpYStack = posYStack[j + 1];
                posYStack[j + 1] = posYStack[j];
                posYStack[j] = tmpYStack;
                if (point[i].accessX() == 0 && point[i].accessY() == 0 && point[i].accessTheta() == 0) {
                    if (isMissed == 1) {
                        cout << "\033[1;31mError :\033[1;32m One Data Point is Missed While Sorting at :\033[0m" << " i : " << i << " / j : " << j << endl;
                        isMissed *= -1;
                    }
                }
                cv::line(tmp2, cv::Point(xCoord[j], yCoord[j]), cv::Point(xCoord[j + 1], yCoord[j + 1]), cv::Scalar(50, 50, 50), 1, 8, 0);
                cv::circle(tmp2, cv::Point(xCoord[j], yCoord[j]), 1, cv::Scalar(0, 255, 0), -1, 8, 0);
                cv::circle(tmp2, cv::Point(xCoord[j + 1], yCoord[j + 1]), 1, cv::Scalar(0, 0, 255), -1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j] - 5, yCoord[j]), cv::Point(xCoord[j] - 7, yCoord[j]), cv::Scalar(255, 255, 255), 1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j] + 5, yCoord[j]), cv::Point(xCoord[j] + 7, yCoord[j]), cv::Scalar(255, 255, 255), 1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j], yCoord[j] + 5), cv::Point(xCoord[j], yCoord[j] + 7), cv::Scalar(255, 255, 255), 1, 8, 0);
                cv::line(tmp2, cv::Point(xCoord[j], yCoord[j] - 5), cv::Point(xCoord[j], yCoord[j] - 7), cv::Scalar(255, 255, 255), 1, 8, 0);
                // cv::imshow("output", tmp2);
                // cv::waitKey(1);
            }
        }
        cv::line(tmp, cv::Point(xCoord[i] - 5, yCoord[i]), cv::Point(xCoord[i] - 10, yCoord[i]), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::line(tmp, cv::Point(xCoord[i] + 5, yCoord[i]), cv::Point(xCoord[i] + 10, yCoord[i]), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::line(tmp, cv::Point(xCoord[i], yCoord[i] + 5), cv::Point(xCoord[i], yCoord[i] + 10), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::line(tmp, cv::Point(xCoord[i], yCoord[i] - 5), cv::Point(xCoord[i], yCoord[i] - 10), cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::circle(tmp, cv::Point(xCoord[i], yCoord[i]), 1, cv::Scalar(255, 255, 255), -1, 8, 0);
    }
    cv::destroyWindow("Sorting Dots by THeir Theta");
    preview.copyTo(tmp);
    int colorValue;
    for (int i = 0; i < amount; i++) {
        colorValue = 200 * i / amount;
        cv::circle(tmp, cv::Point(point[i].accessX(), point[i].accessY()), 1, cv::Scalar(0, 55 + colorValue, 0), -1, 8, 0);
        cv::imshow("output", tmp);
        cv::waitKey(1);
        progressBar(i + 1, "Processing");
        sleep_for(milliseconds(2));
    }
    cv::destroyWindow("Processing");
    for (int i = 0; i < amount; i++) {
        cv::circle(tmp, cv::Point(point[i].accessX(), point[i].accessY()), 1, cv::Scalar(dotBlue, dotGreen, dotRed), -1, 8, 0);
        cv::imshow("output", tmp);
        cv::waitKey(1);
        sleep_for(milliseconds(2));
        progressBar(i + 1, "Preparing to Affect Convex Hull");
    }
    cv::destroyWindow("Preparing to Affect Convex Hull");
    cout << "\033[1;37mSorting Done \033[1;32mSuccessfully\033[0m" << endl << endl;
}

//-- Creates Convex Hull of Generated Dots
void ConvexHull::createConvexHull() {
    cv::Mat tmp;
    cv::Mat tmp2;
    cv::Mat tmp3;
    preview.copyTo(tmp);
    preview.copyTo(tmp2);
    preview.copyTo(tmp3);
    cv::imshow("output", tmp3);
    cout << "\033[1;37mGenerating Convex Hull...\033[0m" << endl;
    for (int i = 0; i < amount; i++) {
        cv::putText(tmp3, to_string(i) + ": " + to_string(point[i].accessX()) + "/" + to_string(point[i].accessY()), cv::Point(point[i].accessX() + 8, point[i].accessY() + 14), cv::FONT_HERSHEY_TRIPLEX, 0.3, cv::Scalar(255, 255, 255));
        cv::imshow("output", tmp3);
        cv::waitKey(1);
    }
    cv::waitKey(0);
    int counter = 0;
    int tmpAmount = amount;
    for (int i = 0; i < amount; i++) {
        tmp.copyTo(tmp2);
        for (int j = 0; j < tmpAmount; j++) {
            tmp2.copyTo(tmp3);
            //- Suggestion Vectors
            // if (point[i].accessTheta() != point)
            cv::circle(tmp3, cv::Point(point[j].accessX(), point[j].accessY()), 10, cv::Scalar(100, 100, 0), 1, 8, 0);
            cv::line(tmp3, cv::Point(point[j].accessX() + 5, point[j].accessY()), cv::Point(point[j].accessX() + 15, point[j].accessY()), cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::line(tmp3, cv::Point(point[j].accessX() - 5, point[j].accessY()), cv::Point(point[j].accessX() - 15, point[j].accessY()), cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::line(tmp3, cv::Point(point[j].accessX(), point[j].accessY() + 5), cv::Point(point[j].accessX(), point[j].accessY() + 15), cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::line(tmp3, cv::Point(point[j].accessX(), point[j].accessY() - 5), cv::Point(point[j].accessX(), point[j].accessY() - 15), cv::Scalar(255, 255, 255), 1, 8, 0);
            cv::putText(tmp3, to_string(determinant(j)), cv::Point(point[j].accessX() + 11, point[j].accessY() + 19), cv::FONT_HERSHEY_TRIPLEX, 0.4, cv::Scalar(0, 255, 255));
            if (j < amount - 1) {
                cv::line(tmp3, cv::Point(point[j].accessX(), point[j].accessY()), cv::Point(point[j + 1].accessX(), point[j + 1].accessY()), cv::Scalar(255, 255, 255), 1, 8, 0);
                // if (j > 0) {
                    cv::line(tmp3, cv::Point(point[j + 1].accessX() + 5, point[j + 1].accessY()), cv::Point(point[j + 1].accessX() + 10, point[j + 1].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
                    cv::line(tmp3, cv::Point(point[j + 1].accessX() - 5, point[j + 1].accessY()), cv::Point(point[j + 1].accessX() - 10, point[j + 1].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
                    cv::line(tmp3, cv::Point(point[j + 1].accessX(), point[j + 1].accessY() + 5), cv::Point(point[j + 1].accessX(), point[j + 1].accessY() + 10), cv::Scalar(0, 100, 255), 1, 8, 0);
                    cv::line(tmp3, cv::Point(point[j + 1].accessX(), point[j + 1].accessY() - 5), cv::Point(point[j + 1].accessX(), point[j + 1].accessY() - 10), cv::Scalar(0, 100, 255), 1, 8, 0);
                // }
            }
            if (j < amount - 2) {
                cv::line(tmp3, cv::Point(point[j].accessX(), point[j].accessY()), cv::Point(point[j + 2].accessX(), point[j + 2].accessY()), cv::Scalar(255, 255, 255), 1, 8, 0);
                // if (j > 0) {
                    cv::line(tmp3, cv::Point(point[j + 2].accessX() + 5, point[j + 2].accessY()), cv::Point(point[j + 2].accessX() + 10, point[j + 2].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
                    cv::line(tmp3, cv::Point(point[j + 2].accessX() - 5, point[j + 2].accessY()), cv::Point(point[j + 2].accessX() - 10, point[j + 2].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
                    cv::line(tmp3, cv::Point(point[j + 2].accessX(), point[j + 2].accessY() + 5), cv::Point(point[j + 2].accessX(), point[j + 2].accessY() + 10), cv::Scalar(0, 100, 255), 1, 8, 0);
                    cv::line(tmp3, cv::Point(point[j + 2].accessX(), point[j + 2].accessY() - 5), cv::Point(point[j + 2].accessX(), point[j + 2].accessY() - 10), cv::Scalar(0, 100, 255), 1, 8, 0);
                // }
            }
            if (determinant(j) < 0) {
                if (j < amount - 1) {
                    cv::line(tmp3, cv::Point(point[j].accessX(), point[j].accessY()), cv::Point(point[j + 1].accessX(), point[j + 1].accessY()), cv::Scalar(0, 200, 0));
                    cv::line(tmp2, cv::Point(point[j].accessX(), point[j].accessY()), cv::Point(point[j + 1].accessX(), point[j + 1].accessY()), cv::Scalar(255, 255, 0));
                    cv::circle(tmp2, cv::Point(point[j + 1].accessX(), point[j + 1].accessY()), 10, cv::Scalar(0, 255, 255), 1, 8, 0);
                    cout << "blink : <--" << endl;
                    if (counter - amount -2) {
                        xConvex[counter] = point[j + 1].accessX();
                        yConvex[counter] = point[j + 1].accessY();
                        // xConvex.insert(counter, point[j + 1].accessX());
                        // xConvex.insert(point[j + 1].accessX());
                        // yConvex.insert(counter ,point[j + 1].accessY());
                        // thetaConvex.insert(counter ,point[j + 1].accessTheta());
                        cout << "Index\033[1;32m "<< counter << " : \033[1;36m(" << point[j + 1].accessX() << "|" << point[j + 1].accessY() << ") \033[0mHas Been Stored" << endl;
                        cout << "Index\033[1;32m "<< counter << " : \033[1;36m(" << xConvex[counter] << "|" << yConvex[counter] << ") \033[0mHas Been Stored" << endl;
                    }
                    counter++;
                }
            } else if (determinant(j) > 0) {
                if (j < amount - 2) {
                    cv::line(tmp3, cv::Point(point[j].accessX(), point[j].accessY()), cv::Point(point[j + 2].accessX(), point[j + 2].accessY()), cv::Scalar(0, 0, 200));
                    cv::line(tmp2, cv::Point(point[j].accessX(), point[j].accessY()), cv::Point(point[j + 2].accessX(), point[j + 2].accessY()), cv::Scalar(255, 255, 0));
                    cv::circle(tmp2, cv::Point(point[j + 2].accessX(), point[j + 2].accessY()), 10, cv::Scalar(0, 255, 255), 1, 8, 0);
                    cv::circle(tmp2, cv::Point(point[j + 1].accessX(), point[j + 1].accessY()), 10, cv::Scalar(0, 0, 255), 1, 8, 0);
                    cout << "blink : -->" << endl;
                    if (counter < amount - 2) {
                        xConvex[counter] = point[j + 2].accessX();
                        yConvex[counter] = point[j + 2].accessY();
                        // xConvex.insert(counter ,point[j + 2].accessX());
                        // yConvex.insert(counter ,point[j + 2].accessY());
                        // thetaConvex.insert(counter ,point[j + 2].accessTheta());
                        cout << "Index\033[1;32m "<< counter << " : \033[1;36m(" << point[j + 2].accessX() << "|" << point[j + 2].accessY() << ") \033[0mHas Been Stored" << endl;
                        cout << "Index\033[1;32m "<< counter << " : \033[1;36m(" << xConvex[counter] << "|" << yConvex[counter] << ") \033[0mHas Been Stored" << endl;
                    }
                    j++;
                    counter++;
                }
            } else if (determinant(j) == 0) {

            }
            // cout << "Index\033[1;32m "<< counter - 1 << " : \033[1;36m(" << xConvex[j] << "|" << yConvex[j] << ") \033[0mHas Been Stored" << endl;
            imshow("output", tmp3);
            cv::waitKey(1);
        }
        for (int j = 0; j < counter; j++) {
            point[j].setX(xConvex[j]);
            point[j].setY(yConvex[j]);
        }
        tmpAmount = counter;
        counter = 0;
        cv::waitKey(1);
        tmp.copyTo(tmp2);
        // imshow("output", tmp2);
    }


    
    // int index = 0;
    // for (int counter = 0; counter < amount; counter++) {
    //     progressBar(counter + 1, "Creating Convex Hull");
    //     index = 0;
    //     tmp3.copyTo(tmp2);
    //     for (int i = 0; i < amount; i++) {
    //         if (!(point[i].accessX() == 0 && point[i].accessY() == 0 && point[i].accessTheta() == 0) && !(point[i + 1].accessX() == 0 && point[i + 1].accessY() == 0 && point[i + 1].accessTheta() == 0) && !(point[i + 2].accessX() == 0 && point[i + 2].accessY() == 0 && point[i + 2].accessTheta() == 0)) {
    //             tmp.copyTo(tmp2);
    //             cv::circle(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), 10, cv::Scalar(255, 255, 0), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i].accessX() + 5, point[i].accessY()), cv::Point(point[i].accessX() + 15, point[i].accessY()), cv::Scalar(255, 100, 0), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i].accessX() - 5, point[i].accessY()), cv::Point(point[i].accessX() - 15, point[i].accessY()), cv::Scalar(255, 100, 0), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY() + 5), cv::Point(point[i].accessX(), point[i].accessY() + 15), cv::Scalar(255, 100, 0), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY() - 5), cv::Point(point[i].accessX(), point[i].accessY() - 15), cv::Scalar(255, 100, 0), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i + 1].accessX() + 5, point[i + 1].accessY()), cv::Point(point[i + 1].accessX() + 10, point[i + 1].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i + 1].accessX() - 5, point[i + 1].accessY()), cv::Point(point[i + 1].accessX() - 10, point[i + 1].accessY()), cv::Scalar(0, 100, 255), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i + 1].accessX(), point[i + 1].accessY() + 5), cv::Point(point[i + 1].accessX(), point[i + 1].accessY() + 10), cv::Scalar(0, 100, 255), 1, 8, 0);
    //             cv::line(tmp2, cv::Point(point[i + 1].accessX(), point[i + 1].accessY() - 5), cv::Point(point[i + 1].accessX(), point[i + 1].accessY() - 10), cv::Scalar(0, 100, 255), 1, 8, 0);
    //             // cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(0, 90, 90), 1, 8, 0);
    //             // cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(0, 90, 90), 1, 8, 0);
    //             if (determinant(i) <= 0) {
    //                 cv::circle(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), 8, cv::Scalar(0, 180, 0), 1, 8, 0);
    //                 cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(255, 0, 0), 1, 8, 0);
    //                 cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(0, 0, 255), 1, 8, 0);
    //                 // cv::line(tmp2, cv::Point(point[i]))
    //                 convexStackX[index] = point[i].accessX();
    //                 convexStackY[index] = point[i].accessY();
    //                 index++;
    //                 cv::waitKey(0);
    //             } else {
    //                 convexStackX[index] = point[i].accessX();
    //                 convexStackY[index] = point[i].accessY();
    //                 index++;
    //                 cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 1].accessX(), point[i + 1].accessY()), cv::Scalar(0, 255, 0), 1, 8, 0);
    //                 cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(0, 255, 0), 1, 8, 0);
    //                 cv::line(tmp, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(120, 120, 120), 1, 8, 0);
    //                 cv::line(tmp2, cv::Point(point[i].accessX(), point[i].accessY()), cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), cv::Scalar(70, 70, 70), 1, 8, 0);
    //                 cv::circle(tmp, cv::Point(point[i].accessX(), point[i].accessY()), 8, cv::Scalar(0, 180, 0), 1, 8, 0);
    //                 cv::circle(tmp, cv::Point(point[i + 2].accessX(), point[i + 2].accessY()), 8, cv::Scalar(0, 180, 0), 1, 8, 0);
    //                 //- Pas the Clockwised Point
    //                 i += 1;
    //             }
    //         }
    //         cv::imshow("output", tmp2);
    //         cv::waitKey(1); 
    //     }

    //     cv::line(tmp2, cv::Point(point[0].accessX(), point[0].accessY()), cv::Point(point[amount - 1].accessX(), point[amount - 1].accessY()), cv::Scalar(255, 0, 0), 1, 8, 0);
    //     int size;
    //     for (int i = 0; i < amount; i++) {
    //         if (i > index) {
    //             // amount = i;
    //         } else {
    //             size = i;
    //             point[i].setX(convexStackX[i]);
    //             point[i].setY(convexStackY[i]);
    //         }
    //     }
    //     tmp.copyTo(tmp2);
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
int ConvexHull::determinant(int i) {
    int vX = point[i + 1].accessX() - point[i].accessX();
    int vY = point[i + 1].accessY() - point[i].accessY();
    int wX = point[i + 2].accessX() - point[i].accessX();
    int wY = point[i + 2].accessY() - point[i].accessY();
    int output = vX * wY - vY * wX;
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