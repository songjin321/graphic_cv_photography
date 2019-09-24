#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;
void reduceVector(cv::Mat &v, vector<uchar> status)
{
    int j = 0;
    for (int i = 0; i < v.rows; i++)
        if (status[i])
            v.row(i).copyTo(v.row(j++));
    v = v.rowRange(0, j);
}

int main()
{
    cv::Mat descriptors;
    cv::Mat onerow = (Mat_<uchar>(1,4) << 1,0,2,3);
    cv::Mat sencondrow = (Mat_<uchar>(1,4) << 1,3,0,2);
    cv::Mat thirdrow = (Mat_<uchar>(1,4) << 2,3,1,0);
    cv::Mat fourthrow = (Mat_<uchar>(1,4) << 3,1,0,2);
    descriptors.push_back(onerow);
    descriptors.push_back(sencondrow);
    descriptors.push_back(thirdrow);
    descriptors.push_back(fourthrow);
    vector<uchar> status = {0,1,1,1};
    cout << descriptors << endl;
    reduceVector(descriptors, status);
    // descriptors.row(0) = fourthrow;
    cout << descriptors << endl;

    descriptors.release();

    descriptors.push_back(thirdrow);
    descriptors.push_back(fourthrow);
    cout << descriptors << endl;
}