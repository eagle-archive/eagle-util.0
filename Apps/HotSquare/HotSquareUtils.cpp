#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <string>
#include <fstream>
#include <hash_set>
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
        if (SEGMENTS_CSV_READ_LIMIT > 0
            && segments.size() >= SEGMENTS_CSV_READ_LIMIT) {
            break;
        }
#endif
    }
    in.close();
    return !segments.empty();
}

SQUARE_ID_T CoordinateToSquareId(const COORDINATE_T *pCoord)
{
    unsigned int hi = (unsigned int)(pCoord->lat * LAT_DEGREE_TO_METER / SQUARE_LAT_SPAN + 0.5);
    unsigned int low = (unsigned int)(pCoord->lng * LNG_DEGREE_TO_METER / SQUARE_LNG_SPAN + 0.5);
    unsigned long long id = ((unsigned long long)hi << 32) | low;
    return id;
}

void SquareIdToCoordinate(SQUARE_ID_T id, COORDINATE_T *pCoord)
{
    pCoord->lat = (unsigned int)(id >> 32) * (double)SQUARE_LAT_SPAN / LAT_DEGREE_TO_METER;
    pCoord->lng = (unsigned int)id * (double)SQUARE_LNG_SPAN / LNG_DEGREE_TO_METER;
    return;
}

static const double LAT_MARGIN = 20.0 / LAT_DEGREE_TO_METER;
static const double LNG_MARGIN = 20.0 / LNG_DEGREE_TO_METER;
static const double LAT_STEP = 2.0 / LAT_DEGREE_TO_METER;
static const double LNG_STEP = 2.0 / LNG_DEGREE_TO_METER;

bool GetSegmentNeighboringSquareIds(const SEGMENT_T *pSegment, vector<SQUARE_ID_T> &squareIds)
{
    double lat1 = pSegment->from.lat;
    double lng1 = pSegment->from.lng;
    double lat2 = pSegment->to.lat;
    double lng2 = pSegment->to.lng;
    if (lat1 > lat2) {
        double t = lat1; lat1 = lat2; lat2 = t;
    }
    if (lng1 > lng2) {
        double t = lng1; lng1 = lng2; lng2 = t;
    }

    lat1 -= LAT_MARGIN;
    lat2 += LAT_MARGIN;
    lng1 -= LNG_MARGIN;
    lng2 += LNG_MARGIN;

    COORDINATE_T coord;
    squareIds.clear();
    for (coord.lat = lat1; coord.lat <= lat2; coord.lat += LAT_STEP) {
        for (coord.lng = lng1; coord.lng < lng2; coord.lng += LNG_STEP) {
            SQUARE_ID_T id = CoordinateToSquareId(&coord);
            bool found = false;
            for (int i=squareIds.size()-1; i>=0; i--) {
                if (squareIds[i] == id) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                squareIds.push_back(id);
            }
        }
    }

    return !squareIds.empty();
}

bool CalculateSquareIds(const SEGMENT_T segments[], int count, hash_set<SQUARE_ID_T> &squareIdSet)
{
    vector<SQUARE_ID_T> squareIds;
    stdext::hash_set<SQUARE_ID_T>::iterator itEnd = squareIdSet.end();

    squareIdSet.clear();
    for (int i=0; i<count; i++) {
        squareIds.clear();
        GetSegmentNeighboringSquareIds(&segments[i], squareIds);

        for (int k=(int)squareIds.size()-1; k>=0; k--) {
            if (squareIdSet.find(squareIds[k]) == itEnd) {
                squareIdSet.insert(squareIds[k]);
            }
        }
    }

    return !squareIdSet.empty();
}


typedef struct MyData {
    SEGMENT_T *pSegStart;
    int nSegCount;
    stdext::hash_set<SQUARE_ID_T> squareIdSet;
} THREAD_DATA;

static DWORD WINAPI MyThreadFunction( LPVOID lpParam ) 
{ 
    THREAD_DATA *pData = (THREAD_DATA *)lpParam;;

    CalculateSquareIds(pData->pSegStart, pData->nSegCount, pData->squareIdSet);

    return 0; 
}

bool CalculateSquareIds_Multi(std::vector<SEGMENT_T> segments, int nThreadCount, stdext::hash_set<SQUARE_ID_T> &squareIdSet)
{
    if (nThreadCount <= 0 || segments.size() == 0) {
        printf("CalculateSquareIds_Multi: invalid parameter passed in\n");
        return false;
    }

    vector<THREAD_DATA> dataArray;
    vector<DWORD> dwThreadIdArray;
    vector<HANDLE> hThreadArray;

    dataArray.resize(nThreadCount);
    dwThreadIdArray.resize(nThreadCount);
    hThreadArray.resize(nThreadCount);

    const int nAverageCount = int(segments.size() / (double)nThreadCount + 0.5);
    for (int i = 0; i < nThreadCount; i++) {
        dataArray[i].pSegStart = segments.data() + nAverageCount * i;
        dataArray[i].nSegCount = nAverageCount;
        if (i == nThreadCount - 1) {
            dataArray[i].nSegCount = segments.size() - nAverageCount * i;
        }

        hThreadArray[i] = ::CreateThread(NULL, 0, MyThreadFunction, &dataArray[i], 0, &dwThreadIdArray[i]);

    }

    // Wait until all threads have terminated.
    ::WaitForMultipleObjects(nThreadCount, hThreadArray.data(), TRUE, INFINITE);
    for(int i=0; i<nThreadCount; i++) {
        CloseHandle(hThreadArray[i]);
    }

    // combine the result set
    squareIdSet.clear();
    const auto squareIdSet_End = squareIdSet.end();

    for (int i=0; i<nThreadCount; i++) {
        stdext::hash_set<SQUARE_ID_T> &subSet = dataArray[i].squareIdSet;
        const auto subSet_End = subSet.end();
        for (stdext::hash_set<SQUARE_ID_T>::iterator it = subSet.begin(); it != subSet_End; it++) {
            if (squareIdSet_End == squareIdSet.find(*it)) {
                squareIdSet.insert(*it);
            }
        }
    }

    return !squareIdSet.empty();
}

std::string FormatTimeStr(unsigned int uTimeMs)
{
    char buff[64];
    sprintf(buff, "%02d:%02d:%03d",
        (uTimeMs/60000) % 100, (uTimeMs/1000) % 100, uTimeMs % 1000);
    return buff;
}

std::string ElapsedTimeStr() {
    static unsigned int s_dwStart = ::GetTickCount();
    return FormatTimeStr(::GetTickCount() - s_dwStart);
}
