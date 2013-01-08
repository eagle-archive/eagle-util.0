#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Constants

// define it to 0 to read all segments
#define SEGMENTS_CSV_READ_LIMIT  0
#define SEGMENTS_CSV_PATH   "Data\\WAY_SEGMENTS\\data"

// Below two defines are specific to Harbin
// Refer to http://www.hhlink.com/%E7%BB%8F%E7%BA%AC%E5%BA%A6
//          http://blog.csdn.net/e_wsq/article/details/7551376
#define LAT_METERS_PER_DEGREE   111190
#define LNG_METERS_PER_DEGREE   77628

// square size (in meters)
#define SQUARE_LAT_SPAN     10
#define SQUARE_LNG_SPAN     10

// Device [0, 360) into some heading levels
// E.g., for 8 heading levels: 0: [-22.5, 22.5), Level 1: [22.5, 45+22.5), ...
// NOTE: make sure it can be divided by 360
#define HEADING_LEVEL_NUM   32

#define TILE_ZOOM_LEVEL  17
#define TOTAL_TILE_NUM  (2 << (TILE_ZOOM_LEVEL - 1))

#define SEG_ASSIGN_DISTANCE_MAX     120 // in meter

// Check if there is C++11 support
#if !(defined(_MSC_VER) && _MSC_VER < 1600)
#define CPP11_SUPPORT
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FormatTimeStr(unsigned int uTimeMs);
std::string ElapsedTimeStr();
double GetDistanceInMeter(double lat1, double lng1, double lat2, double lng2);


#endif // HOT_SQUARE_H_
