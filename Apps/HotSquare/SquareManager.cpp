#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <fstream>
#include "HotSquare.h"
#include "SquareManager.h"

using namespace std;


static const double LAT_MARGIN = 60.0 / LAT_METERS_PER_DEGREE;
static const double LNG_MARGIN = 60.0 / LNG_METERS_PER_DEGREE;
static const double LAT_STEP = 2.0 / LAT_METERS_PER_DEGREE;
static const double LNG_STEP = 2.0 / LNG_METERS_PER_DEGREE;

static
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
            SQUARE_ID_T id = SquareManager::CoordinateToSquareId(coord);
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


bool GenerateSquareIds(const SEGMENT_T segments[], int count, hash_set<SQUARE_ID_T> &squareIdSet)
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


typedef struct {
    SEGMENT_T *pSegStart;
    int nSegCount;
    hash_set<SQUARE_ID_T> squareIdSet;
} THREAD_DATA1;

static unsigned long WINAPI ThreadFun_GenSquareIds( LPVOID lpParam ) 
{ 
    THREAD_DATA1 *pData = (THREAD_DATA1 *)lpParam;;
    GenerateSquareIds(pData->pSegStart, pData->nSegCount, pData->squareIdSet);
    return 0; 
}

bool GenerateSquareIds_Multi(const SEGMENT_T segments[], int nSegs,
    int nThreadCount, hash_set<SQUARE_ID_T> &squareIdSet)
{
    if (segments == NULL || nThreadCount <= 0 || nSegs == 0) {
        printf("CalculateSquareIds_Multi: invalid parameter passed in\n");
        return false;
    }

    vector<THREAD_DATA1> dataArray;
    vector<DWORD> dwThreadIdArray;
    vector<HANDLE> hThreadArray;

    dataArray.resize(nThreadCount);
    dwThreadIdArray.resize(nThreadCount);
    hThreadArray.resize(nThreadCount);

    const int nAverageCount = int(nSegs / (double)nThreadCount + 0.5);
    for (int i = 0; i < nThreadCount; i++) {
        dataArray[i].pSegStart = (SEGMENT_T *)segments + nAverageCount * i;
        dataArray[i].nSegCount = nAverageCount;
        if (i == nThreadCount - 1) {
            dataArray[i].nSegCount = nSegs - nAverageCount * i;
        }

        hThreadArray[i] = ::CreateThread(NULL, 0, ThreadFun_GenSquareIds,
            &dataArray[i], 0, &dwThreadIdArray[i]);
    }

    // Wait until all threads have terminated.
    ::WaitForMultipleObjects(nThreadCount, hThreadArray.data(), TRUE, INFINITE);
    for(int i=0; i<nThreadCount; i++) {
        CloseHandle(hThreadArray[i]);
    }

    // combine the result set
    squareIdSet.clear();
    const decltype(squareIdSet.end()) squareIdSet_End = squareIdSet.end();

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

static void SquareSetToArray(hash_set<SQUARE_ID_T> &squareIdSet, vector<SQUARE_ID_T> &arrIds)
{
    arrIds.reserve(squareIdSet.size());
    for (auto it = squareIdSet.begin(); it != squareIdSet.end(); it++) {
        arrIds.push_back(*it);
    }
}

static bool GenerateSquareArray(TileManager &tileMgr, SQUARE_ID_T squareIds[], int num,
    vector<SQUARE_T> &squareArr)
{
    squareArr.clear();
    squareArr.reserve(num);

    SQUARE_T sq;
    for (int i = 0; i < num; i++) {
        sq.square_id = squareIds[i];
        if ((i % 100000) == 0) {
            printf("%s: Pre-calculate for square ID %d\n", ElapsedTimeStr().c_str(), i);
        }

        COORDINATE_T centerCoord;
        SquareManager::SquareIdToCenterCoordinate(sq.square_id, &centerCoord);
        SEG_ID_T seg_id_heading_levels[HEADING_LEVEL_COUNT];
        // Get seg ID for each heading for the coordinate
        for (int level = HEADING_LEVEL_COUNT - 1; level >= 0; level--) {
            seg_id_heading_levels[level] =
                tileMgr.AssignSegment(centerCoord, level * (360 / HEADING_LEVEL_COUNT));
        }

        sq.arr_headings_seg_id.clear();
        sq.arr_headings_seg_id.reserve(HEADING_LEVEL_COUNT);

        for (int i1 = 0; i1 < HEADING_LEVEL_COUNT;) {
            int i2 = i1;
            while(i2 < HEADING_LEVEL_COUNT && seg_id_heading_levels[i2] == seg_id_heading_levels[i1]) {
                i2++;
            }
            i2--;
            if (seg_id_heading_levels[i1] != 0) {
                HEADINGS_TO_SEG_IDS_T heads_segid;
                heads_segid.from_level = i1;
                heads_segid.to_level = i2;
                heads_segid.seg_id = seg_id_heading_levels[i1];
                sq.arr_headings_seg_id.push_back(heads_segid);
            }
            i1 = i2 + 1;
        }

        sq.arr_headings_seg_id.shrink_to_fit();
        squareArr.push_back(sq);
    }

    return !squareArr.empty();
}

bool SquareManager::BuildSquareMap_Multi(SegManager &segMgr, TileManager &tileMgr, int nThreadCount)
{
    mpSegMgr = &segMgr;
    mpTileMgr = &tileMgr;

    hash_set<SQUARE_ID_T> squareIdSet;
    bool res = GenerateSquareIds_Multi(segMgr.GetSegArray(), segMgr.GetSegArrayCount(),
        nThreadCount, squareIdSet);
    if (res == false)
        return res;
    printf("%s: GenerateSquareIds_Multi, result: %d\n", ElapsedTimeStr().c_str(), squareIdSet.size());

    //printf("%s: before SetToArray\n", ElapsedTimeStr().c_str());
    vector<SQUARE_ID_T> squareIdArr;
    SquareSetToArray(squareIdSet, squareIdArr);
    squareIdSet.clear();
    //printf("%s: after SetToArray\n", ElapsedTimeStr().c_str());

    vector<SQUARE_T> squareArr;
    GenerateSquareArray(tileMgr, squareIdArr.data(), squareIdArr.size(), squareArr);
    squareIdArr.clear();

    // build mSquareMap
    mSquareMap.clear();
    for (int i = 0; i < (int)squareArr.size(); i++) {
        SQUARE_T *pSq = new SQUARE_T;
        *pSq = squareArr[i];
        mSquareMap.insert(SQUARE_MAP_T::value_type(pSq->square_id, pSq));
    }

    return !mSquareMap.empty();
}

void SquareManager::ClearSquareMap()
{
    for (auto it = mSquareMap.begin(); it != mSquareMap.end(); it++) {
        if (it->second != NULL) {
            delete it->second;
            it->second = NULL;
        }
    }
    mSquareMap.clear();
}

bool SquareManager::SaveToCsvFile(const char *filename)
{
    std::ofstream out(filename);
    if (!out.good())
        return false;

    for (auto it = mSquareMap.begin(); it != mSquareMap.end(); it++) {
        SQUARE_T *pSq = it->second;
        for (size_t i = 0; i < pSq->arr_headings_seg_id.size(); i++) {
            char buff[512];
            // "seqare id, heading_from, heading_to, segment id"
            sprintf(buff, "%lld,%d,%d,%lld\n", pSq->square_id,
                pSq->arr_headings_seg_id[i].from_level, pSq->arr_headings_seg_id[i].to_level,
                pSq->arr_headings_seg_id[i].seg_id);
            out << buff;
        }
    }
    out.close();
    return true;
}
