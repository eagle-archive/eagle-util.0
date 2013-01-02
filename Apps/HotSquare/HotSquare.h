#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Constants

// to save debug time, define it to 0 to read all segments
#define SEGMENTS_CSV_READ_LIMIT  0//1000

// Below two defines are specific to Harbin
// Refer to http://www.hhlink.com/%E7%BB%8F%E7%BA%AC%E5%BA%A6
//          http://blog.csdn.net/e_wsq/article/details/7551376
#define LAT_METERS_PER_DEGREE   111190
#define LNG_METERS_PER_DEGREE   77628

#define SEGMENTS_CSV_PATH   "Data\\WAY_SEGMENTS\\data"

// square size (in meters)
#define SQUARE_LAT_SPAN     7
#define SQUARE_LNG_SPAN     7

// To change zoom level, update ZOOM_LEVEL & TOTAL_TILE_NUM
#define ZOOM_LEVEL      ZOOM_18
#define TOTAL_TILE_NUM  TOTAL_TILE_18

// Predefined constants, DO NOT CHANGE!
#define ZOOM_16         16
#define TOTAL_TILE_16   65536  // 2^16
#define ZOOM_17         17
#define TOTAL_TILE_17   131072 // 2^17
#define ZOOM_18         18
#define TOTAL_TILE_18   262144 // 2^18

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FormatTimeStr(unsigned int uTimeMs);
std::string ElapsedTimeStr();


#endif // HOT_SQUARE_H_
