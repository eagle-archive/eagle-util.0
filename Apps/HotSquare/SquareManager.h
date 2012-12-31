#ifndef SQUARE_SQUARE_H_
#define SQUARE_SQUARE_H_

#include "HotSquare.h"
#include "TileManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// class SquareManager

typedef unsigned long long SQUARE_ID_T;

typedef struct {
    // low 32 bit from lng coordinate, hi 32 bit from lat coordinate
    SQUARE_ID_T square_id;
    SEG_ID_T seg_id1;
    unsigned int heading1;
    SEG_ID_T seg_id2;
    unsigned int heading2;
} SQUARE_T, *P_SQUARE_T;

typedef std::map<SQUARE_ID_T, P_SQUARE_T> SQUARE_MAP_T;

class SquareManager {
public:
    SquareManager() { mpSegMgr = NULL; };
    ~SquareManager() { ClearSquareMap();};

    bool BuildSquareMap_Multi(SegManager &segMgr, int nThreadCount);

    SQUARE_MAP_T &GetSquareMap() {
        return mSquareMap;
    };
    int GetSquareCount() {
        return mSquareMap.size();
    };
    static inline SQUARE_ID_T CoordinateToSquareId(const COORDINATE_T *pCoord);
    static inline void SquareIdToCenterCoordinate(SQUARE_ID_T id, COORDINATE_T *pCoord);

private:
    void ClearSquareMap();

    SegManager  *mpSegMgr;
    SQUARE_MAP_T mSquareMap;
};

SQUARE_ID_T SquareManager::CoordinateToSquareId(const COORDINATE_T *pCoord) {
    unsigned int hi = (unsigned int)(pCoord->lat * LAT_METERS_PER_DEGREE / SQUARE_LAT_SPAN + 0.5);
    unsigned int low = (unsigned int)(pCoord->lng * LNG_METERS_PER_DEGREE / SQUARE_LNG_SPAN + 0.5);
    return ((unsigned long long)hi << 32) | low;
}

void SquareManager::SquareIdToCenterCoordinate(SQUARE_ID_T id, COORDINATE_T *pCoord) {
    pCoord->lat = (unsigned int)(id >> 32) * (double)SQUARE_LAT_SPAN / LAT_METERS_PER_DEGREE;
    pCoord->lng = (unsigned int)id * (double)SQUARE_LNG_SPAN / LNG_METERS_PER_DEGREE;
}

#endif // SQUARE_SQUARE_H_
