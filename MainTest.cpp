#include <string>
#include <iostream>
#include "coordinate_convert_tool.h"

int main(int argc, char **argv)
{
	//设置参数，lat_0 地图原点经度坐标(标注北纬(n)还是南纬(s))，lon_0 地图原点纬度坐标，字符串的书写方式严格按照此标准，不可随意增加空格
	std::string WGS84 = "+proj=longlat +datum=WGS84 +no_defs";
	std::string TMERC = "+proj=tmerc +lat_0=22.6656342253927n  +lon_0=114.054429823102 +ellps=WGS84 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs";
	CoordinateConvertTool *convert_coordiante = new CoordinateConvertTool(WGS84, TMERC);
	
  //经纬度转为地图坐标，转换出来的结果应与原点坐标(0,0)，注意传入的数据类型
  float x = 0.0;
  float y = 0.0;
  convert_coordiante->ConvertLonLat2Cartesian(114.054429823102, 22.6656342253927, &x, &y);
  printf("x = %0.5lf\n", x);
  printf("y = %0.5lf\n", y);
  
  //地图坐标转为经纬度，转换出来的结果应与原点经纬度坐标一致，注意传入的数据类型
 	double lon = 0.0;//单位，度
  double lat = 0.0;//单位，度
 	convert_coordiante->ConvertCartesian2LonLat(0.0, 0.0, &lon, &lat);
 	printf("lon_0 = 114.054429823102\n");
 	printf("lat_0 = 22.6656342253927\n");
 	printf("lon = %0.15lf\n", lon);
 	printf("lat = %0.15lf\n", lat);

 	

  if(convert_coordiante)
  {
  	delete convert_coordiante;
  	convert_coordiante = nullptr;
  }

  
	return 0;
}