#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <fstream>
#include "HotSquare.h"

using namespace std;


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

static inline TILE_ID_T CoordToTileId(const COORDINATE_T &coord) {
    unsigned int hi = int(coord.lat * ((double)LAT_METERS_PER_DEGREE / TILE_SPAN) + 0.5);
    unsigned int low = int(coord.lng * ((double)LNG_METERS_PER_DEGREE / TILE_SPAN) + 0.5);
    return ((unsigned long long)hi << 32) | low;
}

/*
 * If the tile ID is new, create a new tile for it and insert it to the tile map.
 * If the file ID is in tile map, update the related segment ID
*/
static inline void CheckUpdateTile(TILE_MAP_T &tileMap, const TILE_ID_T &tileId,
    const SEG_ID_T &segId) {

    TILE_MAP_T::iterator it = tileMap.find(tileId);
    TILE_T *pTile;

    if (it == tileMap.end()) {
        pTile = new TILE_T;
        pTile->tile_id = tileId;
        pTile->segIdsSet.insert(segId);
        tileMap.insert(TILE_MAP_T::value_type(tileId, pTile));
    } else {
        pTile = it->second;
        if (pTile->segIdsSet.find(segId) == pTile->segIdsSet.end()) {
            pTile->segIdsSet.insert(segId);
        }
    }
}



// return number of neighboring tiles
// NOTE: count of neiTiles[] must be 8
static inline int GetNeighborTiles(TILE_MAP_T &tileMap, TILE_T *pThisTile, TILE_PTR_T neiTiles[]) {
    unsigned int low = (unsigned int)pThisTile->tile_id;
    unsigned int hi = (unsigned int)(pThisTile->tile_id >> 32);

#define MAKE_TILE_ID(low, hi) ((TILE_ID_T)(low) | ((TILE_ID_T)(hi) << 32))

    TILE_ID_T idNeighbors[] = {
        MAKE_TILE_ID(low-1, hi-1), MAKE_TILE_ID(low, hi-1), MAKE_TILE_ID(low+1, hi-1),
        MAKE_TILE_ID(low-1, hi),                            MAKE_TILE_ID(low+1, hi),
        MAKE_TILE_ID(low-1, hi+1), MAKE_TILE_ID(low, hi+1), MAKE_TILE_ID(low+1, hi+1)};

    int count = 0;
    for (int i = 0; i < sizeof(idNeighbors)/idNeighbors[0]; i++) {
        TILE_T *pTile = tileMap[idNeighbors[i]];
        if (pTile) {
            neiTiles[count] = pTile;
            count++;
        }
    }

    return 0;
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

static inline void UpdateTileForNeighborSegs(TILE_MAP_T &tileMap, TILE_T *pTile) {
    pTile->segsWithNeighbors.clear();
    pTile->segsWithNeighbors.reserve(pTile->segIdsSet.size() * 3);

    // copy the segments into pTile->segsWithNeighbors
    for (hash_set<SEG_ID_T>::iterator it = pTile->segIdsSet.begin();
        it != pTile->segIdsSet.end(); it++) {
            pTile->segsWithNeighbors.push_back(*it);
    }

    // copy the neighboring segments into pTile->segsWithNeighbors
    TILE_PTR_T neiTiles[8];
    int count = GetNeighborTiles(tileMap, pTile, neiTiles);
    for (int i = 0; i < count; i++) {
        AddToSegsNoDuplicate(pTile->segsWithNeighbors, neiTiles[i]->segIdsSet);
    }
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

    for (auto it = mTileMap.begin(); it != mTileMap.end(); it++) {
        UpdateTileForNeighborSegs(mTileMap, it->second);
    }

    return !mTileMap.empty();
}
