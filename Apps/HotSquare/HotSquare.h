#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#include <vector>

typedef struct {
    double lat;
    double lng;
} COORDINATE_T;

typedef struct {
    unsigned int seg_id;
    COORDINATE_T from;
    COORDINATE_T to;
    unsigned int way_id;
    unsigned int length;
    int weight;
} SEGMENT_T;

bool ReadFromCSV(const char *path, std::vector<SEGMENT_T> &segments);

#endif // HOT_SQUARE_H_
