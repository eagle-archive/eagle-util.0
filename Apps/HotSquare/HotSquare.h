#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#include <vector>
#include <map>
#include <hash_set>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Constants

// to save debug time, define it to 0 to read all segments
#define SEGMENTS_CSV_READ_LIMIT  0//10000

// Below two defines are specific to Harbin
// Refer to http://www.hhlink.com/%E7%BB%8F%E7%BA%AC%E5%BA%A6
//          http://blog.csdn.net/e_wsq/article/details/7551376
#define LAT_METERS_PER_DEGREE   111190
#define LNG_METERS_PER_DEGREE   77628

#define SEGMENTS_CSV_PATH   "Data\\WAY_SEGMENTS\\data"

// square size (in meter)
#define SQUARE_LAT_SPAN     7
#define SQUARE_LNG_SPAN     7

#define TILE_SPAN   200 // tile span in meters


////////////////////////////////////////////////////////////////////////////////////////////////////
// class SegManager
/*
CREATE ROW TABLE "HEB_OSM"."WAY_SEGMENTS"  (
    "ID" BIGINT CS_FIXED DEFAULT 0 NOT NULL,
    "FROM_LAT" DOUBLE CS_DOUBLE NOT NULL, "FROM_LNG" DOUBLE CS_DOUBLE NOT NULL,
    "TO_LAT" DOUBLE CS_DOUBLE NOT NULL, "TO_LNG" DOUBLE CS_DOUBLE NOT NULL,
    "WAY_ID" BIGINT CS_FIXED NOT NULL,
    "ONE_WAY" TINYINT CS_INT DEFAULT 0 NOT NULL,
    "LENGTH" DOUBLE CS_DOUBLE DEFAULT 0 NOT NULL,
    "WEIGHT" DOUBLE CS_DOUBLE DEFAULT 0 ) 
*/

typedef struct {
    double lat;
    double lng;
} COORDINATE_T;

typedef unsigned long long SEG_ID_T;

typedef struct {
    SEG_ID_T seg_id;
    COORDINATE_T from;
    COORDINATE_T to;
    unsigned long long way_id;
    short int one_way;
    double length;
    double weight;
} SEGMENT_T;

class SegManager {
public:
    SegManager();
    ~SegManager();

    SEGMENT_T *GetSegArray();
    int GetSegArrayCount() const;
    bool LoadFromCsvFile(const char *path);
    const SEGMENT_T *GetSegByID(const SEG_ID_T &segId) {
        SEG_ID_MAP::iterator it = mSegIdMap.find(segId);
        return (it == mSegIdMap.end()) ? NULL : &mAllSegs[it->second];
    };

private:
    typedef std::map<SEG_ID_T, int> SEG_ID_MAP;
    SEG_ID_MAP mSegIdMap; // map seg ID to the index of mAllSegs
    std::vector<SEGMENT_T> mAllSegs;
};

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

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FormatTimeStr(unsigned int uTimeMs);
std::string ElapsedTimeStr();


#endif // HOT_SQUARE_H_
