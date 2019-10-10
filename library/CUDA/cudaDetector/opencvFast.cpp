#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include "TicToc.hpp"



int main(int argc, char **argv)
{
    cv::Ptr<cv::FastFeatureDetector> fastDetector = cv::FastFeatureDetector::create(100, true, 2);
    cv::Ptr<cv::cuda::FastFeatureDetector> gpuFastDetector = cv::cuda::FastFeatureDetector::create(100, true, 2);

    std::vector<cv::KeyPoint> keypoint;
    std::vector<cv::KeyPoint> gpu_keypoint;

    cv::Mat frame;
    cv::cuda::GpuMat gFrame;

    frame = cv::imread("data/img1.png");
    cv::cvtColor(frame, frame, CV_BGR2GRAY);
    gFrame.upload(frame);

    TicToc fast_cpu_timer;
    fastDetector->detect(gFrame, keypoint);
    std::cout << " fast cpu detection cost time :  " << fast_timer.toc();

    TicToc fast_gpu_timer;
    gpuFastDetector->detect(gFrame, keypoint);
    std::cout << " fast gpu detection cost time :  " << fast_timer.toc();

}