/**
 * @file ceres_views.cpp
 * @author Song Jin (songjinxs@163.com)
 * @brief This cpp is a implementation of using ceres library to solve the max infomation view.
 * @version 0.1
 * @date 2019-10-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include <eigen3/Eigen/Geometry>

struct CostFunctor{

    CostFunctor(double info, Eigen::Vector3d w_p):
    info_(info), w_p_(w_p)
    {
    }

    template <typename T>
    bool operator()(const T* const w_c_view, T* residuals) const
    {
        T c_p[3];
        T w_p_para[3];
        w_p_para[0] = T(w_p_[0]);
        w_p_para[1] = T(w_p_[1]);
        w_p_para[2] = T(w_p_[2]);
        ceres::QuaternionRotatePoint(w_c_view, w_p_para, c_p);

        T normal_z = c_p[2]/(sqrt(c_p[0] * c_p[0] + c_p[1] * c_p[1] + c_p[2] * c_p[2]));
        residuals[0] = T(info_) * exp(T(5.0) - T(5.0) * normal_z);

        return true;
    }

    static ceres::CostFunction* Create(const double info, const Eigen::Vector3d w_p)
    {
        return (new ceres::AutoDiffCostFunction<CostFunctor, 1, 4>(
            new CostFunctor(info, w_p)
        ));
    }
private:
    double info_; // 路标点对当前位置相机的信息量
    Eigen::Vector3d w_p_; // 路标点在世界坐标的位置
};

int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    ceres::LocalParameterization* local_parameterization = new ceres::QuaternionParameterization();
    double w_c_view_para[4];
    w_c_view_para[0] = 0.653;
    w_c_view_para[1] = -0.653;
    w_c_view_para[2] = 0.271;
    w_c_view_para[3] = -0.271;

    ceres::Problem problem;
    problem.AddParameterBlock(w_c_view_para, 4, local_parameterization);

    size_t kNPts = 1;
    
    std::vector<double> infos = {1.0};
    std::vector<Eigen::Vector3d> w_ps = {{1.0, 1.0, 1.0}};
    for (size_t pt_i = 0; pt_i < kNPts; pt_i++)
    {
        ceres::CostFunction* cost_function = CostFunctor::Create(infos[pt_i], w_ps[pt_i]);
        problem.AddResidualBlock(cost_function, NULL, w_c_view_para);
    }
	ceres::Solver::Options options;
	ceres::Solver::Summary summary;
	ceres::Solve(options, &problem, &summary);

    std::cout << summary.FullReport() << "\n";
    
    Eigen::Quaterniond result(w_c_view_para[0], w_c_view_para[1], w_c_view_para[2], w_c_view_para[3]);
    std::cout << "result = \n" << w_c_view_para[0] << " " << w_c_view_para[1] << " "<< w_c_view_para[2] << " " << w_c_view_para[3] << " "<< std::endl;
    Eigen::Quaterniond a;
    return 0;
}