#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <string>
#include "HotSquare.h"

using namespace std;



std::string FormatTimeStr(unsigned int uTimeMs)
{
    char buff[64];
    sprintf(buff, "%2d:%02d:%03d",
        (uTimeMs/60000), (uTimeMs/1000) % 60, uTimeMs % 1000);
    return buff;
}

static unsigned int g_dwStart = ::GetTickCount();
std::string ElapsedTimeStr() {
    return FormatTimeStr(::GetTickCount() - g_dwStart);
}


static inline double rad(double d)
{
    return d * M_PI / 180.0;
}

// Refer to http://blog.sina.com.cn/s/blog_65d859c30101akih.html
double GetDistanceInMeter(double lat1, double lng1, double lat2, double lng2)
{
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lng1) - rad(lng2);
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    return s * 6378137; // 6378137 is earth radius in meter
}
