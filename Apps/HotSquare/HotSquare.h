#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Constants

// to save debug time, define it to 0 to read all segments
#define SEGMENTS_CSV_READ_LIMIT  3000

// Below two defines are specific to Harbin
// Refer to http://www.hhlink.com/%E7%BB%8F%E7%BA%AC%E5%BA%A6
//          http://blog.csdn.net/e_wsq/article/details/7551376
#define LAT_METERS_PER_DEGREE   111190
#define LNG_METERS_PER_DEGREE   77628

#define SEGMENTS_CSV_PATH   "Data\\WAY_SEGMENTS\\data"

// square size (in meter)
#define SQUARE_LAT_SPAN     7
#define SQUARE_LNG_SPAN     7

#define TILE_SPAN   200 // tile span in meters

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FormatTimeStr(unsigned int uTimeMs);
std::string ElapsedTimeStr();


#endif // HOT_SQUARE_H_
