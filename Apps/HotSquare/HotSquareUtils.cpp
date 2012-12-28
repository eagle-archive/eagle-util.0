// HotSquare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include "HotSquare.h"

using namespace std;


static
bool get_line(std::ifstream &fs, std::string &line) {
    line.clear();
    do{
        if(getline(fs, line)) {
            if(fs.good() && line.empty()){
                continue;
            }
            return true;
        } else {
            return false;
        }
    } while(true);
    return false;
}

bool ReadFromCsv(const char *path, vector<SEGMENT_T> &segments)
{
    segments.clear();
    std::ifstream in(path);
    std::string line;

    while (get_line(in, line)) {
        SEGMENT_T seg;
        int one_way;

        int r = sscanf(line.c_str(), "%lld,%lf,%lf,%lf,%lf,%lld,%d,%lf,%lf",
            &seg.seg_id, &seg.from.lat, &seg.from.lng, &seg.to.lat, &seg.to.lng,
            &seg.way_id, &one_way, &seg.length, &seg.weight);
        if (r == 9) {
            seg.one_way = one_way;
            segments.push_back(seg);
        } else {
            printf("incorrect line in CSV: %s\n", line.c_str());
        }

#ifdef SEGMENTS_CSV_READ_LIMIT
        if (segments.size() >= SEGMENTS_CSV_READ_LIMIT) {
            break;
        }
#endif
    }

    return !segments.empty();
}

unsigned long long CoordinateToSquareId(const COORDINATE_T *pCoord)
{
    unsigned int hi = (unsigned int)(pCoord->lat * LAT_DEGREE_TO_METER / SQUARE_LAT_SPAN + 0.5);
    unsigned int low = (unsigned int)(pCoord->lng * LNT_DEGREE_TO_METER / SQUARE_LNG_SPAN + 0.5);
    unsigned long long id = (unsigned long long)hi << 32 | low;
    return id;
}

void SquareIdToCoordinate(unsigned long long id, COORDINATE_T *pCoord)
{
    unsigned int hi = (unsigned int)(id >> 32);
    unsigned int low = (unsigned int)id;
    pCoord->lat = hi * (double)SQUARE_LAT_SPAN / LAT_DEGREE_TO_METER;
    pCoord->lng = low * (double)SQUARE_LNG_SPAN / LNT_DEGREE_TO_METER;
}
