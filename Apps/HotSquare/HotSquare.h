#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#include <vector>

// to save debug time, need to be comment out for real computation
#define SEGMENTS_CSV_READ_LIMIT  1000

#define LAT_DEGREE_TO_METER     65355
#define LNT_DEGREE_TO_METER     111190


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
unsigned long long CoordinateToSquareId(const COORDINATE_T *pCoord);
void SquareIdToCoordinate(unsigned long long id, COORDINATE_T *pCoord);

#endif // HOT_SQUARE_H_
