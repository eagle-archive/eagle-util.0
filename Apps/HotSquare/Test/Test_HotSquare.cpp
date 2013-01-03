
#include "../HotSquare.h"
#include "../SquareManager.h"

using namespace std;

extern SegManager gSegManager;
extern TileManager gTileManager;

bool Test_CoordinateMapping()
{
    COORDINATE_T coord, coord2;
    coord.lat = 0.000500;
    coord.lng = 0.000500;
    unsigned long long id = SquareManager::CoordinateToSquareId(coord);
    SquareManager::SquareIdToCenterCoordinate(id, &coord2);
    return true;
}

void PrintCoord(const char *str, const COORDINATE_T &coord)
{
    printf("%s: %lf, %lf\n", str, coord.lng, coord.lat);
}

void PrintTileId(const char *str, const TILE_ID_T &tileId)
{
    printf("%s: 0x%llX, %lld\n", str, tileId, tileId);
}

void PrintSquareId(const char *str, const SQUARE_ID_T &sId)
{
    printf("%s: 0x%llX, %lld\n", str, sId, sId);
}

bool Test_TileManager()
{
    {
        COORDINATE_T coord;
        coord.lng = 126.602420;
        coord.lat = 45.720608;
        TILE_ID_T tileId = TileManager::CoordToTileId(coord);
        PrintCoord("coord", coord);
        printf("tile ID: 0x%llX\n", tileId);

        TileManager::TileIdToCenterCoord(tileId, &coord);
        PrintCoord("coord", coord);
    }
    {
        COORDINATE_T coord;
        coord.lng = 126.602420;
        coord.lat = 45.720608;
        PrintCoord("coord", coord);
        TILE_T *pTile = gTileManager.GetTileByCoord(coord);
        if (pTile == NULL)
            return false;

        COORDINATE_T coord1, coord2;
        TileManager::GetTileCoordinates(pTile->tile_id, &coord1, &coord2);
        PrintCoord("coord1", coord1);
        PrintCoord("coord2", coord2);
    }
    return true;
}

bool Test_TileManager_SampleDataAssignment()
{
    SQUARE_ID_T sId;

    COORDINATE_T coord;
    coord.lng = 126.63172;
    coord.lat = 45.76266;
    int heading = 61;
    SEG_ID_T seg_id_expected = 15352;
    sId = SquareManager::CoordinateToSquareId(coord);
    SEG_ID_T assignedId = gTileManager.AssignSegment(coord, heading);

    coord.lng = 126.63961;
    coord.lat = 45.78882;
    heading = 102;
    seg_id_expected = 20925;
    sId = SquareManager::CoordinateToSquareId(coord);
    assignedId = gTileManager.AssignSegment(coord, heading);

    return true;
}

bool Test_SegManager_Distance()
{
    COORDINATE_T coord;
    coord.lng = 126.63961;
    coord.lat = 45.78882;
    const SEGMENT_T *pSeg1 = gSegManager.GetSegByID(20925); // should be of minimal distance?
    const SEGMENT_T *pSeg2 = gSegManager.GetSegByID(20923);
    double distance1 = gSegManager.CalcDistance(coord, *pSeg1);
    double distance2 = gSegManager.CalcDistance(coord, *pSeg2);

    return true;
}

bool Test_Main()
{
    if (false == Test_CoordinateMapping()) {
        printf("Test_CoordinateMapping failed!\n");
    }
    if (false == Test_TileManager()) {
        printf("Test_TileManager failed!\n");
    }
    //while(1) Test_SegManager_Distance();
    Test_TileManager_SampleDataAssignment();
    return true;
}
