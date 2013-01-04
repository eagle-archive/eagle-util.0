#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <fstream>
#include "TileManager.h"

using namespace std;

#ifndef COUNT_OF
#define COUNT_OF(a) (sizeof(a)/sizeof(*a))
#endif

#define MAKE_TILE_ID(low, hi) ((TILE_ID_T)(low) | ((TILE_ID_T)(hi) << 32))

#define DECLARE_NEIGHBOR_IDS(arr_name, tile_id) \
        int _lng_ = (int)(tile_id); \
        int _lat_ = (int)((tile_id) >> 32); \
        TILE_ID_T arr_name[] = { \
            MAKE_TILE_ID(_lng_-1, _lat_-1), MAKE_TILE_ID(_lng_, _lat_-1), MAKE_TILE_ID(_lng_+1, _lat_-1), \
            MAKE_TILE_ID(_lng_-1, _lat_),                                 MAKE_TILE_ID(_lng_+1, _lat_), \
            MAKE_TILE_ID(_lng_-1, _lat_+1), MAKE_TILE_ID(_lng_, _lat_+1), MAKE_TILE_ID(_lng_+1, _lat_+1) \
        }

static inline bool InSameDirection(int heading1, int heading2) {
    int diff = (heading2 + 360 - heading1) % 360;
    return diff < 90 || diff > 270;
}

void TileManager::ClearTileMap()
{
    for (TILE_MAP_T::iterator it = mTileMap.begin(); it != mTileMap.end(); it++) {
        if (it->second != NULL) {
            delete it->second;
            it->second = NULL;
        }
    }
    mTileMap.clear();
}

// return number of neighboring tiles
// NOTE: count of neiTiles[] must be 8
static inline int GetNeighborTiles(TILE_MAP_T &tileMap, TILE_T *pThisTile, P_TILE_T neiTiles[]) {
    DECLARE_NEIGHBOR_IDS(idNeighbors, pThisTile->tile_id);
    int count = 0;
    for (int i = 0; i < COUNT_OF(idNeighbors); i++) {
        auto it = tileMap.find(idNeighbors[i]);
        if (it != tileMap.end()) {
            neiTiles[count] = it->second;
            count++;
        }
    }

    return count;
}

/*
 * If the tile ID is new, create a new tile for it and insert it to the tile map.
 * If the tile ID is already in tile map, update the related segment ID
*/
static inline void CheckUpdateTile(TILE_MAP_T &tileMap, const TILE_ID_T &tileId,
    const SEG_ID_T &segId) {

    TILE_MAP_T::iterator it = tileMap.find(tileId);
    if (it == tileMap.end()) {
        TILE_T *pTile = new TILE_T();
        pTile->tile_id = tileId;
        pTile->segIdsSet.insert(segId);
        tileMap.insert(TILE_MAP_T::value_type(tileId, pTile));
    } else {
        // The tile for the tile ID already in the map, segment ID => segment ID set
        TILE_T *pTile = it->second;
        if (pTile->segIdsSet.find(segId) == pTile->segIdsSet.end()) {
            pTile->segIdsSet.insert(segId);
        }
    }
}

static inline void AddToSegsNoDuplicate(std::vector<SEG_ID_T> &segs, hash_set<SEG_ID_T> &segIdsSet) {
    for (hash_set<SEG_ID_T>::iterator it = segIdsSet.begin(); it != segIdsSet.end(); it++) {
        const SEG_ID_T &segId = *it;
        bool found = false;
        for (int i = (int)segs.size() - 1; i >= 0; i--) {
            if (segId == segs[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            segs.push_back(segId);
        }
    }
}

// Make sure this tile has 8 neighboring tiles, even though some of them are empty.
// This is because in this way, if a point falls on one of the neighboring tiles (e.g., E2),
// it can still find "This" tile to get the segment.
//  E1   E2   E3
//  E4   This E5
//  E6   E7   E8
static inline void CheckAddEmptyNeighborTiles(TILE_MAP_T &tileMap, TILE_ID_T tileId) {
    DECLARE_NEIGHBOR_IDS(nbTileIdArray, tileId);
    for (int i = 0; i < COUNT_OF(nbTileIdArray); i++) {
        auto it = tileMap.find(nbTileIdArray[i]);
        if (it == tileMap.end()) {
            // Simply inser an empty tile for the neighbor
            TILE_T *pTile = new TILE_T();
            pTile->tile_id = nbTileIdArray[i];
            tileMap.insert(TILE_MAP_T::value_type(nbTileIdArray[i], pTile));
        }
    }
}

// This function only update pTile->segsWithNeighbors[]
static inline void UpdateTileForNeighborSegs(TILE_MAP_T &tileMap, TILE_T *pTile) {
    pTile->segsWithNeighbors.clear();
    pTile->segsWithNeighbors.reserve(pTile->segIdsSet.size() * 3);

    // copy the segments into pTile->segsWithNeighbors
    for (auto it = pTile->segIdsSet.begin(); it != pTile->segIdsSet.end(); it++) {
        pTile->segsWithNeighbors.push_back(*it);
    }

    // copy the neighboring segments into pTile->segsWithNeighbors
    P_TILE_T nbTiles[8];
    int count = GetNeighborTiles(tileMap, pTile, nbTiles);
    for (int i = 0; i < count; i++) {
        AddToSegsNoDuplicate(pTile->segsWithNeighbors, nbTiles[i]->segIdsSet);
    }

    pTile->segsWithNeighbors.shrink_to_fit();
}

bool TileManager::GenerateTiles(SegManager &mSegMgr)
{
    mpSegMgr = &mSegMgr;
    if (mpSegMgr->GetSegArrayCount() == 0)
        return false;

    SEGMENT_T *allSegments = mpSegMgr->GetSegArray();
    const int nSegCount = mpSegMgr->GetSegArrayCount();

    for (int i = 0; i < nSegCount; i++) {
        const SEGMENT_T &seg = allSegments[i];
        TILE_ID_T tileId1 = CoordToTileId(seg.from);
        TILE_ID_T tileId2 = CoordToTileId(seg.to);
        if (tileId1 == tileId2) {
            CheckUpdateTile(mTileMap, tileId1, seg.seg_id);
        } else {
            CheckUpdateTile(mTileMap, tileId1, seg.seg_id);
            CheckUpdateTile(mTileMap, tileId2, seg.seg_id);
        }
    }

    {
        vector<TILE_ID_T> arrTileIds;
        arrTileIds.reserve(mTileMap.size());
        for (auto it = mTileMap.begin(); it != mTileMap.end(); it++) {
            arrTileIds.push_back(it->first);
        }
        for (int i = (int)arrTileIds.size() - 1; i >= 0; i--) {
            CheckAddEmptyNeighborTiles(mTileMap, arrTileIds[i]);
        }
    }

    for (auto it = mTileMap.begin(); it != mTileMap.end(); it++) {
        UpdateTileForNeighborSegs(mTileMap, it->second);
    }

    return !mTileMap.empty();
}

bool TileManager::SaveToCsvFile(const char *filename)
{
    std::ofstream out(filename);
    if (!out.good())
        return false;

    for (TILE_MAP_T::iterator it = mTileMap.begin(); it != mTileMap.end(); it++) {
        COORDINATE_T coord1, coord2;
        GetTileCoordinates(it->second->tile_id, &coord1, &coord2);

        char buff[1024];
        sprintf(buff, "0x%llX,%lf,%lf,%lf,%lf\n", it->second->tile_id,
            coord1.lat, coord1.lng, coord2.lat, coord2.lng);
        out << buff;
    }
    out.close();
    return true;
}

void TileManager::GetBoundingBox(const TILE_ID_T &tileId,
    double &north, double &south, double &east, double &west)
{
    north = LatIdToLat((unsigned int)(tileId >> 32));
    south = LatIdToLat((unsigned int)(tileId >> 32) + 1);
    west = LngIdToLng((unsigned int)tileId);
    east = LngIdToLng((unsigned int)tileId + 1);
}

void TileManager::GetTileCoordinates(const TILE_ID_T &tileId, COORDINATE_T *pCoord1, COORDINATE_T *pCoord2)
{
    if (pCoord1 == NULL && pCoord2 == NULL)
        return;

    double north, south, east, west;
    GetBoundingBox(tileId, north, south, east, west);

    if (pCoord1) {
        pCoord1->lat = north;
        pCoord1->lng = west;
    }
    if (pCoord2) {
        pCoord2->lat = south;
        pCoord2->lng = east;
    }
}

SEG_ID_T TileManager::AssignSegment(const COORDINATE_T &coord, int nHeading)
{
    TILE_ID_T tileId = TileManager::CoordToTileId(coord);
    auto it = mTileMap.find(tileId);
    if (it == mTileMap.end())
        return false;

    TILE_T *pTile = it->second;
    auto arrSegs = pTile->segsWithNeighbors;

    double distanceMin = DBL_MAX;
    int minIndex = -1;

    for (size_t i = 0; i < arrSegs.size(); i++) {
        const SEGMENT_T *pSeg = mpSegMgr->GetSegByID(arrSegs[i]);

        // If not the same direction, ignore
        if (pSeg != NULL && InSameDirection((int)(pSeg->heading + 0.5), nHeading)) {
            // get the min distance
            double distance = SegManager::CalcDistance(coord, *pSeg);
            if (distance < distanceMin) {
                minIndex = i;
                distanceMin = distance;
            }
        }
    }

    return minIndex < 0 ? 0 : arrSegs[minIndex];
}
