#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <iostream>
#include "cudaImage.h"
#include "cudaSift.h"
#include <ctime>
#include <cstdlib>
#include <chrono>

class TicToc
{
  public:
    TicToc()
    {
        tic();
    }

    void tic()
    {
        start = std::chrono::system_clock::now();
    }

    double toc()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        return elapsed_seconds.count() * 1000;
    }

  private:
    std::chrono::time_point<std::chrono::system_clock> start, end;
};

using namespace cv;  
using namespace std;
using namespace cv::xfeatures2d;

// Extract Sift features from images
SiftData siftData;
float initBlur = 1.0f;
float thresh = 2.0f;
Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
void imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        cv::Mat colorImg = cv_ptr->image;
        cv::Mat grayImg, limg;
        cv::cvtColor(colorImg, grayImg, cv::COLOR_BGR2GRAY);
        grayImg.convertTo(limg, CV_32FC1);

        unsigned int w = limg.cols;
        unsigned int h = limg.rows;
        std::cout << "Image size = (" << w << "," << h << ")" << std::endl;

        // Initial Cuda images and download images to device
        std::cout << "Initializing data..." << std::endl;
        InitCuda(0);
        CudaImage img;
        img.Allocate(w, h, iAlignUp(w, 128), false, NULL, (float *)limg.data);
        img.Download();

        // A bit of benchmarking
        //for (int thresh1=1.00f;thresh1<=4.01f;thresh1+=0.50f) {
        float *memoryTmp = AllocSiftTempMemory(w, h, 5, false);

        ExtractSift(siftData, img, 5, initBlur, thresh, 0.0f, false, memoryTmp);
        FreeSiftTempMemory(memoryTmp);
        std::cout << "Number of original features: " << siftData.numPts << std::endl;
        
        // opencv sift detection
        TicToc opencv_sift_timer;
        vector<KeyPoint> sift_keypoints_opencv;
        f2d->detect(grayImg, sift_keypoints_opencv);
        cout << "the scale of keypoint 0: " << sift_keypoints_opencv[0].octave << endl;
        cout << "Opencv Sift Detection Costs: " << opencv_sift_timer.toc() << " ms" << endl;
        cout << "The keypoints number of img1 is:" << sift_keypoints_opencv.size() << endl;
        
        // draw points
        std::vector<cv::KeyPoint> sift_keypoints;
        SiftPoint *sift_points = siftData.h_data;
        for (size_t i = 0; i < siftData.numPts; i++)
        {
            cv::KeyPoint keypoint;
            keypoint.pt.x = sift_points[i].xpos;
            keypoint.pt.y = sift_points[i].ypos;
            keypoint.response = sift_points[i].score;
            keypoint.angle = sift_points[i].orientation;
            keypoint.octave = sift_points[i].scale;
            sift_keypoints.push_back(keypoint);
        }
        cv::Mat drawImg;
        cv::drawKeypoints(colorImg, sift_keypoints_opencv, drawImg, cv::Scalar(0, 0, 255));
        cv::imshow("Detected Sift Feature", drawImg);
        cv::waitKey(2);
    }
    catch (cv_bridge::Exception &e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "sift_test_node");
    ros::NodeHandle nh;
    InitSiftData(siftData, 32768, true, true);
    ros::Subscriber image_sub = nh.subscribe("/d435i/color/image_raw", 1, imageCallback);
    ros::spin();
    return 0;
}