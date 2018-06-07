

#ifndef COORDINATE_CONVERT_TOOL_H
#define COORDINATE_CONVERT_TOOL_H

#include "Eigen/Dense"
#include <string>
#include <proj_api.h>

#define LOCAL_DEG_TO_RAD (M_PI/180.0)
//heading pitch roll in some data file is radian
#define AccGravity 9.7925 //the acceleration of gravity in shenzhen

class CoordinateConvertTool
{
public:
    CoordinateConvertTool(const std::string &src_string, const std::string &dst_string);
    ~CoordinateConvertTool();

    bool ConvertLonLat2Cartesian(const double &longitude, const double &latitude, float *x, float *y);
    bool ConvertCartesian2LonLat(const float &x, const float &y, double *longitude, double *latitude);

    Eigen::Matrix3d GetRotationMatrix(double heading, double pitch, double roll);

    Eigen::Matrix4d GetIMU2MapRTMatrix( const float &x, const float &y, const float &z,
                                        const float &heading, const float &pitch, const float &roll);

private:
    bool SetParam();
    bool paramSetFlags_;
    std::string src_convert_param_;
    std::string dst_convert_param_;
    projPJ pj_latlong_;
    projPJ pj_utm_;
    static std::string WGS84;
    static std::string UTM;
    static std::string TMERC;
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // COORDINATE_CONVERT_TOOL_H
