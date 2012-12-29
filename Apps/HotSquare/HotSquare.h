#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#include <vector>
#include <hash_set>

// to save debug time, define it to 0 to read all segments
#define SEGMENTS_CSV_READ_LIMIT  10000

// Below two defines are specific to Harbin
// Refer to http://www.hhlink.com/%E7%BB%8F%E7%BA%AC%E5%BA%A6
//          http://blog.csdn.net/e_wsq/article/details/7551376
#define LAT_METERS_PER_DEGREE   111190
#define LNG_METERS_PER_DEGREE   77628


#define SEGMENTS_CSV_PATH   "Data\\WAY_SEGMENTS\\data"

// square size (in meter)
#define SQUARE_LAT_SPAN     7
#define SQUARE_LNG_SPAN     7


/*
CREATE ROW TABLE "HEB_OSM"."WAY_SEGMENTS"  (
    "ID" BIGINT CS_FIXED DEFAULT 0 NOT NULL,
    "FROM_LAT" DOUBLE CS_DOUBLE NOT NULL, "FROM_LNG" DOUBLE CS_DOUBLE NOT NULL,
    "TO_LAT" DOUBLE CS_DOUBLE NOT NULL, "TO_LNG" DOUBLE CS_DOUBLE NOT NULL,
    "WAY_ID" BIGINT CS_FIXED NOT NULL,
    "ONE_WAY" TINYINT CS_INT DEFAULT 0 NOT NULL,
    "LENGTH" DOUBLE CS_DOUBLE DEFAULT 0 NOT NULL,
    "WEIGHT" DOUBLE CS_DOUBLE DEFAULT 0 ) 
*/

typedef struct {
    double lat;
    double lng;
} COORDINATE_T;

typedef unsigned long long SQUARE_ID_T;

typedef struct {
    unsigned long long seg_id;
    COORDINATE_T from;
    COORDINATE_T to;
    unsigned long long way_id;
    short int one_way;
    double length;
    double weight;
} SEGMENT_T;

bool ReadFromCsv(const char *path, std::vector<SEGMENT_T> &segments);
SQUARE_ID_T CoordinateToSquareId(const COORDINATE_T *pCoord);
void SquareIdToCoordinate(SQUARE_ID_T id, COORDINATE_T *pCoord);
bool GetSegmentNeighboringSquareIds(const SEGMENT_T *pSegment, std::vector<SQUARE_ID_T> &squareIds);
bool CalculateSquareIds(const SEGMENT_T segments[], int count, stdext::hash_set<SQUARE_ID_T> &squareIdSet);
bool CalculateSquareIds_Multi(std::vector<SEGMENT_T> segments, int nThreadCount, stdext::hash_set<SQUARE_ID_T> &squareIdSet);

std::string FormatTimeStr(unsigned int uTimeMs);
std::string ElapsedTimeStr();

#endif // HOT_SQUARE_H_
