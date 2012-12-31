
#include "../HotSquare.h"

using namespace std;

extern SegManager gSegManager;
extern TileManager gTileManager;

bool Test_CoordinateMapping()
{
    COORDINATE_T coord, coord2;
    coord.lat = 0.000500;
    coord.lng = 0.000500;
    unsigned long long id = SquareManager::CoordinateToSquareId(&coord);
    SquareManager::SquareIdToCenterCoordinate(id, &coord2);
    return true;
}

bool Test_CoordinateToTile()
{
    COORDINATE_T coord;
    coord.lat = 45.720608;
    coord.lng = 126.602420;
    TILE_T *pTile = gTileManager.GetTileByCoord(coord);

    return pTile != NULL;
}

bool Test_Main()
{
    if (false == Test_CoordinateMapping()) {
        printf("Test_CoordinateMapping failed!");
    }
    if (false == Test_CoordinateToTile()) {
        printf("Test_CoordinateToTile failed!");
    }

    return true;
}
