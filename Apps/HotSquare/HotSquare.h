#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#include <vector>

#define CSV_READ_LIMIT  1000 // to save debug time


typedef struct {
    double lat;
    double lng;
} COORDINATE_T;

typedef struct {
    unsigned int seg_id;
    COORDINATE_T from;
    COORDINATE_T to;
    unsigned long long way_id;
    short int one_way;
    double length;
    double weight;
} SEGMENT_T;

bool ReadFromCSV(const char *path, std::vector<SEGMENT_T> &segments);

#endif // HOT_SQUARE_H_
