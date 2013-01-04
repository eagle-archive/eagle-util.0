#ifndef SQUARE_SQUARE_H_
#define SQUARE_SQUARE_H_

#include "HotSquare.h"
#include "SegManager.h"
#include "TileManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// class SquareManager

typedef unsigned long long SQUARE_ID_T;

typedef struct {
    int         from_level;
    int         to_level;
    SEG_ID_T    seg_id;
} HEADINGS_TO_SEG_IDS_T;

typedef struct {
    // low 32 bit from lng coordinate, hi 32 bit from lat coordinate
    SQUARE_ID_T square_id;
    // Array of segment IDs for all the heading levels
    std::vector<HEADINGS_TO_SEG_IDS_T> arr_headings_seg_id;
} SQUARE_T, *P_SQUARE_T;

typedef std::map<SQUARE_ID_T, P_SQUARE_T> SQUARE_MAP_T;

class SquareManager {
public:
    SquareManager() { mpSegMgr = NULL; };
    ~SquareManager() { ClearSquareMap();};

    bool BuildSquareMap_Multi(SegManager &segMgr, TileManager &tileMgr, int nThreadCount);

    SQUARE_MAP_T &GetSquareMap() {
        return mSquareMap;
    };
    int GetSquareCount() {
        return (int)mSquareMap.size();
    };
    SQUARE_T *GetSquareById(const SQUARE_ID_T &id) {
		SQUARE_MAP_T::iterator it = mSquareMap.find(id);
        return (it == mSquareMap.end()) ? NULL : it->second;
    };
    bool SaveToCsvFile(const char *filename);
    SEG_ID_T AssignSegment(const COORDINATE_T &coord, int nHeading); // return 0 if not found

    static inline SQUARE_ID_T CoordinateToSquareId(const COORDINATE_T &coord) {
        unsigned int hi = (unsigned int)(coord.lat * LAT_METERS_PER_DEGREE / SQUARE_LAT_SPAN);
        unsigned int low = (unsigned int)(coord.lng * LNG_METERS_PER_DEGREE / SQUARE_LNG_SPAN);
        return ((unsigned long long)hi << 32) | low;
    };
    static inline void SquareIdToCenterCoordinate(const SQUARE_ID_T &id, COORDINATE_T *pCoord) {
        pCoord->lat = ((unsigned int)(id >> 32) + 0.5) * (double)SQUARE_LAT_SPAN / LAT_METERS_PER_DEGREE;
        pCoord->lng = ((unsigned int)id + 0.5) * (double)SQUARE_LNG_SPAN / LNG_METERS_PER_DEGREE;
    };

private:
    void ClearSquareMap();

    SegManager  *mpSegMgr;
    TileManager *mpTileMgr;
    SQUARE_MAP_T mSquareMap;
};

#endif // SQUARE_SQUARE_H_
