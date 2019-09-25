/**
 * @file geometry.cpp
 * @author Song Jin (songjinxs@163.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <iostream>
#include <eigen3/Eigen/Geometry>
#include <map>
using namespace std;
using namespace Eigen;

int main()
{
    map<int, vector<pair<int, Eigen::Matrix<double, 7, 1>>>> image;
    Eigen::Matrix<double, 7, 1> xyz_uv_velocity;
    xyz_uv_velocity << 1, 1, 1, 1, 1, 1, 1;
    image[2].emplace_back(0,  xyz_uv_velocity);
    image[2].emplace_back(0,  xyz_uv_velocity);
    cout << image[2].size() << endl;
    return 0;
}