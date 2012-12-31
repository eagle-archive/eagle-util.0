#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <fstream>
#include "HotSquare.h"
#include "SegManager.h"


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
                seg.heading = SegManager::GetHeading(seg.from, seg.to);
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
    mAllSegs.shrink_to_fit();
    return !mAllSegs.empty();
}

double SegManager::GetHeading(const COORDINATE_T &coFrom, const COORDINATE_T &coTo)
{
    double heading;
    double x1 = coTo.lng - coFrom.lng;
    double y1 = coTo.lat - coFrom.lat;
    const double x2 = 0;
    const double y2 = 1;
    double cos_value = (x1*x2 + y1*y2) / (sqrt(x1*x1 + y1*y1) * (sqrt(x2*x2 + y2*y2)));
    double delta_radian = acos(cos_value);
    if(x1 > 0) {
        heading = delta_radian * 180.0 / M_PI;
    }else{
        heading = 360.0 - delta_radian * 180.0 / M_PI;
    }
    return heading;
}

double SegManager::CalcDistance(const COORDINATE_T &coord, const SEGMENT_T &seg)
{
    double apx = coord.lng - seg.from.lng;
    double apy = coord.lat - seg.from.lat;
    double abx = seg.to.lng - seg.from.lng;
    double aby = seg.to.lat - seg.from.lat;
    double ab2 = abx * abx + aby * aby;
    double ap_ab = apx * abx + apy * aby;
    double t = ap_ab / ab2;
    if (t < 0) {
        t = 0;
    }
    else if (t > 1) {
        t = 1;
    }

    return (coord.lng - seg.from.lng + abx * t) * (coord.lng - seg.from.lng + abx * t)
        + (seg.from.lat + aby * t) * (seg.from.lat + aby * t);
}
