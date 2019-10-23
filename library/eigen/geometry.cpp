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
#include <typeinfo>
using namespace std;
using namespace Eigen;

int test1()
{
    map<int, vector<pair<int, Eigen::Matrix<double, 7, 1>>>> image;
    Eigen::Matrix<double, 7, 1> xyz_uv_velocity;
    xyz_uv_velocity << 1, 1, 1, 1, 1, 1, 1;
    image[2].emplace_back(0,  xyz_uv_velocity);
    image[2].emplace_back(0,  xyz_uv_velocity);
    cout << image[2].size() << endl;
    return 0;
}

int test2()
{
    Eigen::Quaterniond test(0.653, -0.653, 0.271, -0.271);
    auto result = test * Eigen::Vector3d(0,0,1);
    cout << result << endl;
}

int main(int argc, char** argv)
{
    Eigen::AngleAxis<double> as1;
    Eigen::AngleAxis<double> as2;
    auto as3 = as1 * as2;
    std::cout << "the type of two eigen angleaxis multiple is " << typeid(as3).name() << std::endl;
    test2();
}