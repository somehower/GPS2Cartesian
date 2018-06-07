
#include <stdio.h>
#include "coordinate_convert_tool.h"

//the longitude and latitue is WGS84 sphere coordinate 
std::string CoordinateConvertTool::WGS84 = "+proj=longlat +datum=WGS84 +no_defs";
//we can define the origin coordinate by setup the TMERC param
std::string CoordinateConvertTool::TMERC = "+proj=tmerc +lat_0=22.6656342253927n  +lon_0=114.054429823102 +ellps=WGS84 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs";
//we can use the UTM param, the origin is realated to the "zone"
std::string CoordinateConvertTool::UTM = "+proj=utm +zone=50 +ellps=WGS84 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs";

CoordinateConvertTool::CoordinateConvertTool(const std::string &src_string, const std::string &dst_string)
{
    pj_latlong_     = NULL;
    pj_utm_         = NULL;
    paramSetFlags_  = false;

    src_convert_param_ = src_string;
    dst_convert_param_ = dst_string;

    if(!SetParam())
    {
        printf("set parameter fail, pleas check the parameter.\n");
        src_convert_param_ = WGS84;
        dst_convert_param_ = TMERC;
        SetParam();
    }
}

CoordinateConvertTool::~CoordinateConvertTool()
{
    if(pj_latlong_)
    {
        pj_free(pj_latlong_);
        pj_latlong_ = NULL;
    }
    if(pj_utm_)
    {
        pj_free(pj_utm_);
        pj_utm_ = NULL;
    }
}

bool CoordinateConvertTool::SetParam()
{
    if (pj_latlong_)
    {
        pj_free(pj_latlong_);
        pj_latlong_ = NULL;
    }

    if (pj_utm_)
    {
        pj_free(pj_utm_);
        pj_utm_ = NULL;
    }

    if (!(pj_latlong_ = pj_init_plus(src_convert_param_.c_str())))
    {

        paramSetFlags_ = false;
        return false;
    }

    if (!(pj_utm_= pj_init_plus(dst_convert_param_.c_str())))
    {
        paramSetFlags_ = false;
        pj_free(pj_latlong_);
        pj_latlong_ = NULL;
        return false;
     }
    paramSetFlags_ = true;
    return true;
}

// euler to rotation matrix, units is rad
Eigen::Matrix3d CoordinateConvertTool::GetRotationMatrix(double heading, double pitch, double roll)
{
    Eigen::AngleAxisd yawAngle(heading, Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitY());

    Eigen::Quaterniond q = yawAngle * pitchAngle * rollAngle;
    Eigen::Matrix3d R = q.matrix();//R is RotationMatrix

    return R;
}

//log and lat units is degree, positon units is m
bool CoordinateConvertTool::ConvertLonLat2Cartesian(const double &longitude, const double &latitude, float *x, float *y)
{
    if(paramSetFlags_ == false)
    {
        printf("Please Set parameter first.\n");
        return false;
    }

    double lon = longitude * LOCAL_DEG_TO_RAD;
    double lat = latitude * LOCAL_DEG_TO_RAD;
    
    pj_transform(pj_latlong_, pj_utm_, 1, 1, &lon, &lat, NULL);
    *x = (float)lon;
    *y = (float)lat;
    return true;
}

////log and lat units is degree, positon units is m
bool CoordinateConvertTool::ConvertCartesian2LonLat(const float &x, const float &y, double *longitude, double *latitude)
{
    if(paramSetFlags_ == false)
    {
        printf("Please Set parameter first.\n");
        return false;
    }

    double lon = (double)x;
    double lat = (double)y;
    
    pj_transform(pj_utm_, pj_latlong_, 1, 1, &lon, &lat, NULL);
    *longitude = lon / LOCAL_DEG_TO_RAD;
    *latitude = lat / LOCAL_DEG_TO_RAD;
    return true;
}

//position's units is m, attitude's units is degree
Eigen::Matrix4d CoordinateConvertTool::GetIMU2MapRTMatrix(const float &x, const float &y, const float &z,
                                                          const float &heading, const float &pitch, const float &roll)
{
    // the NovAtel heading's output, north left is negative, north rigth is positive.
    // it doesn't follow the right hand principle of cartesian coordinate, it should plus -1
    Eigen::Matrix3d IMU2LocalRotationMatrix = GetRotationMatrix((double)heading  * LOCAL_DEG_TO_RAD * (-1.0),  
                                                                (double)pitch  * LOCAL_DEG_TO_RAD, 
                                                                (double)roll * LOCAL_DEG_TO_RAD);

    Eigen::Matrix4d IMU2LocalRTMatrix = Eigen::Matrix4d::Identity();

    IMU2LocalRTMatrix.topLeftCorner(3,3) = IMU2LocalRotationMatrix;
    IMU2LocalRTMatrix(0,3) = (double)x;
    IMU2LocalRTMatrix(1,3) = (double)y;
    IMU2LocalRTMatrix(2,3) = (double)z;

    return IMU2LocalRTMatrix;
}


