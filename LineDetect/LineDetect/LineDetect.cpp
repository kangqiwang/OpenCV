//// LineDetect.cpp : Defines the entry point for the console application.
////
//
#include "stdafx.h"

#include <sys/stat.h>
#include "stdlib.h"
#include "LineDetectAlgorithm.h"
#include "MyTool.h"

using namespace cv;
using namespace std;

string realPosition = "./Exact Goal Positions/exactGoalPosition";
string approxPos = "./Approximate Goal Positions/approximateGoalPosition";
string position = "algorithmGoalPosition";
string PositionResults = "GoalPositionAlgorithmResults";

string imgfiles = "./Images/im";
string imagesOutput = "./Output/";
string outputImg = "imageWithGoal";

int main(int argc, char** argv)
{
    //delete original output 
    cout << "try to delete previous output folder if exists" << endl;
    system("rd /s /q .\\Output");
    //Create output directory
    cout << "create new output folder" << endl;
    system("mkdir .\\Output");

    //Result file
    ofstream outputFile;
    string outputFileName = (imagesOutput + PositionResults + ".txt").c_str();
    outputFile.open(outputFileName, ios_base::out);

    for (int i = 1; i <= 20; i++)
    {
        //generate png file name
        stringstream ss;
        ss << setw(4) << setfill('0') << i;
        string number = ss.str();
        string originalImageFileName = imgfiles + number + ".png";
        cout << "Current png is:" << originalImageFileName << endl;
        // Read the png file
        Mat image = imread(originalImageFileName, CV_LOAD_IMAGE_COLOR);  
        if (!image.data)                          
        {
            cout << "Can not open the image" << endl;
            return -1;
        }

        //get postion from txt file
        string exactPosFileName = realPosition + number + ".txt";
        vector<Point2f> exactGoalPos = CMyTool::readPositionsFromTxt(exactPosFileName);
        string approxPosFileName = approxPos + number + ".txt";
        vector<Point2f> approxGoalPos = CMyTool::readPositionsFromTxt(approxPosFileName);

        //find the shift
        LineDetectAlgorithm  LineDetectAlgorithm(image, approxGoalPos);
        Point2f shift = LineDetectAlgorithm.findTheShift();
        for (int i = 0; i < approxGoalPos.size(); i++)
        {
            approxGoalPos[i] += shift;
        }

        //draw goal line
        CMyTool::drawGoalLine(image, approxGoalPos, Scalar(0, 0, 255));

        //write the result into txt file
        outputFile << number << " (" << floor(shift.x*10.0 + 0.5) / 10.0 << " " << floor(shift.y*10.0 + 0.5) / 10.0 << ") " << floor(cv::norm(shift)*10.0 + 0.5) / 10.0 << endl;
        string approxGoalFile = imagesOutput + position + number + ".txt";
        CMyTool::writePositionsToTxt(approxGoalFile, approxGoalPos);

        //save png file
        string outputImageFile = imagesOutput + outputImg + number + ".png";
        imwrite(outputImageFile, image);

    }

    outputFile.close();
    cout << "Image processing Finished" << endl;

    getchar();                                        // Wait for a keystroke in the window
    return 0;
}


