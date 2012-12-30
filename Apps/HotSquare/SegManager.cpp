#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <fstream>
#include "HotSquare.h"

using namespace std;

SegManager::SegManager()
{
}

SegManager::~SegManager()
{
}

SEGMENT_T *SegManager::GetSegArray()
{
    return mAllSegs.data();
}

int SegManager::GetSegArrayCount() const
{
    return mAllSegs.size();
}

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

bool SegManager::LoadFromCsvFile(const char *path)
{
    mAllSegs.clear();
    mAllSegs.reserve(30000);

    std::ifstream in(path);
    std::string line;

    int count = 0, csvLineNum = 0;
    while (get_line(in, line)) {
        SEGMENT_T seg;
        int one_way;

        int r = sscanf(line.c_str(), "%lld,%lf,%lf,%lf,%lf,%lld,%d,%lf,%lf",
            &seg.seg_id, &seg.from.lat, &seg.from.lng, &seg.to.lat, &seg.to.lng,
            &seg.way_id, &one_way, &seg.length, &seg.weight);
        if (r == 9) {
            seg.one_way = one_way;

            // remove duplicated
            if (mSegIdMap.find(seg.seg_id) == mSegIdMap.end()) {
                mAllSegs.push_back(seg);
                mSegIdMap.insert(SEG_ID_MAP::value_type(seg.seg_id, count));
                count++;
            }
        } else {
            printf("incorrect line in CSV: %s\n", line.c_str());
        }

#ifdef SEGMENTS_CSV_READ_LIMIT
        csvLineNum++;
        if (SEGMENTS_CSV_READ_LIMIT > 0
            && csvLineNum >= SEGMENTS_CSV_READ_LIMIT) {
            break;
        }
#endif
    }
    in.close();
    return !mAllSegs.empty();
}

const SEGMENT_T *SegManager::GetSegByID(SEG_ID_T segId)
{
    SEG_ID_MAP::iterator it = mSegIdMap.find(segId);
    return it == mSegIdMap.end() ? NULL : &mAllSegs[it->second];
}
