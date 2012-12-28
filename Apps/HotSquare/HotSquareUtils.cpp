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

bool ReadFromCSV(const char *path, vector<SEGMENT_T> &segments)
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
