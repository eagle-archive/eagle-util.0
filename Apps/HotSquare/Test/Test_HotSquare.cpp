
#include <iostream>
#include "../HotSquare.h"
#include "../SquareManager.h"

#define DMS_TO_DEGREE(d,m,s,ms)    (d + (m)/60.0 + ((s) + (ms)/1000.0)/3600.0)

using namespace std;

extern SegManager gSegManager;
extern TileManager gTileManager;
extern SquareManager gSquareManager;

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

bool Test_TileManager_AssignOnePoint(double lng, double lat, int heading, const SEG_ID_T &expected_id)
{
    COORDINATE_T coord;
    coord.lng = lng;
    coord.lat = lat;

    SEG_ID_T res_id = gTileManager.AssignSegment(coord, heading);
    cout << "expected seg ID: " << expected_id << " assigned ID: " << res_id << "\n";
    return res_id == expected_id;
}

bool Test_TileManager_SampleDataAssignment()
{
    printf("\nTest_TileManager_SampleDataAssignment:\n");
    Test_TileManager_AssignOnePoint(DMS_TO_DEGREE(126,36,51,299), DMS_TO_DEGREE(45,39,46,619), 217, 0);
    Test_TileManager_AssignOnePoint(DMS_TO_DEGREE(126,39,42,803), DMS_TO_DEGREE(45,45,24,299), 272, 9774);
    Test_TileManager_AssignOnePoint(DMS_TO_DEGREE(126,40,45,433), DMS_TO_DEGREE(45,44, 3,418),  56, 2798);
    Test_TileManager_AssignOnePoint(126.70537972222222, 45.75574972222222, 19, 3980);
    Test_TileManager_AssignOnePoint(126.78225972222222, 45.77801972222222, 17, 22197);
    Test_TileManager_AssignOnePoint(126.64852972222224, 45.743409722222225, 22, 12759);
    return true;
}

bool Test_SquareManager_AssignOnePoint(double lng, double lat, int heading, const SEG_ID_T &expected_id)
{
    COORDINATE_T coord;
    coord.lng = lng;
    coord.lat = lat;

    SEG_ID_T res_id = gSquareManager.AssignSegment(coord, heading);
    cout << "expected seg ID: " << expected_id << " assigned ID: " << res_id << "\n";
    return res_id == expected_id;
    return true;
}

bool Test_SquareManager_SampleDataAssignment()
{
    printf("\nTest_SquareManager_SampleDataAssignment:\n");
    Test_SquareManager_AssignOnePoint(DMS_TO_DEGREE(126,36,51,299), DMS_TO_DEGREE(45,39,46,619), 217, 0);
    Test_SquareManager_AssignOnePoint(DMS_TO_DEGREE(126,39,42,803), DMS_TO_DEGREE(45,45,24,299), 272, 9774);
    Test_SquareManager_AssignOnePoint(DMS_TO_DEGREE(126,40,45,433), DMS_TO_DEGREE(45,44, 3,418),  56, 2798);
    Test_SquareManager_AssignOnePoint(126.70537972222222, 45.75574972222222, 19, 3980);
    Test_SquareManager_AssignOnePoint(126.78225972222222, 45.77801972222222, 17, 22197);
    Test_SquareManager_AssignOnePoint(126.64852972222224, 45.743409722222225, 22, 12759);

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
    return distance1 <= distance2;
}

void Test_GetTileSize()
{
    COORDINATE_T coord;
    coord.lng = 126.5288783855;
    coord.lat = 45.8017592384;
    TILE_ID_T tid = TileManager::CoordToTileId(coord);
    double north, south, east, west;
    TileManager::GetBoundingBox(tid, north, south, east, west);

    double m1 = GetDistanceInMeter(north, east, south, east);
    double m2 = GetDistanceInMeter(north, east, north, west);
}

bool Test_Main()
{
    if (false == Test_CoordinateMapping()) {
        printf("Test_CoordinateMapping failed!\n");
    }
    if (false == Test_TileManager()) {
        printf("Test_TileManager failed!\n");
    }
    Test_SegManager_Distance();
    Test_GetTileSize();
    Test_TileManager_SampleDataAssignment();
    Test_SquareManager_SampleDataAssignment();

    return true;
}
