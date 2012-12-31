#ifndef TILE_MANAGER_H_
#define TILE_MANAGER_H_

#include <hash_set>
#include "SegManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// class TileManager

typedef unsigned long long TILE_ID_T;

typedef struct {
    // low 32 bit from lng coordinate, hi 32 bit from lat coordinate
    TILE_ID_T tile_id;
    // hash set for all intersected segments
    stdext::hash_set<SEG_ID_T> segIdsSet;
    // array of segments containing segments from extra 8 neighbor 
    std::vector<SEG_ID_T> segsWithNeighbors;
} TILE_T, *P_TILE_T;

typedef std::map<TILE_ID_T, TILE_T *> TILE_MAP_T;

class TileManager {
public:
    TileManager() {
        mpSegMgr = NULL;
    };
    ~TileManager() {
        ClearTileMap();
    };
    int GetTileCount() const {
        return mTileMap.size();
    };
    TILE_T *GetTileById(const TILE_ID_T &tileId) {
        auto it = mTileMap.find(tileId);
        return it == mTileMap.end() ? NULL : it->second;
    };
    TILE_T *GetTileByCoord(const COORDINATE_T &coord) {
        return GetTileById(CoordToTileId(coord));
    };
    TILE_MAP_T &GetTileMap() {
        return mTileMap;
    };
    bool GenerateTiles(SegManager &segMgr);
    bool SaveToCsvFile(const char *filename);
    SEG_ID_T AssignSegment(const COORDINATE_T &coord, int nHeading); // return 0 if not found

    static void GetTileCoordinates(const TILE_ID_T &tileId, COORDINATE_T *pCoord1, COORDINATE_T *pCoord2);
    static inline TILE_ID_T CoordToTileId(const COORDINATE_T &coord);
    static inline void TileIdToCenterCoord(const TILE_ID_T &tileId, COORDINATE_T *pCoord);

private:
    void ClearTileMap();
    SegManager *mpSegMgr;
    TILE_MAP_T mTileMap;
};

void TileManager::TileIdToCenterCoord(const TILE_ID_T &tileId, COORDINATE_T *pCoord)
{
    pCoord->lat = (unsigned int)(tileId >> 32) * (double)TILE_SPAN / LAT_METERS_PER_DEGREE;
    pCoord->lng = (unsigned int)tileId * (double)TILE_SPAN / LNG_METERS_PER_DEGREE;
}

TILE_ID_T TileManager::CoordToTileId(const COORDINATE_T &coord) {
    unsigned int hi = int(coord.lat * ((double)LAT_METERS_PER_DEGREE / TILE_SPAN) + 0.5);
    unsigned int low = int(coord.lng * ((double)LNG_METERS_PER_DEGREE / TILE_SPAN) + 0.5);
    return ((unsigned long long)hi << 32) | low;
}

#endif // TILE_MANAGER_H_
