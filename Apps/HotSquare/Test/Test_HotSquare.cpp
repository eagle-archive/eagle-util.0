
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
    unsigned long long id = SquareManager::CoordinateToSquareId(&coord);
    SquareManager::SquareIdToCenterCoordinate(id, &coord2);
    return true;
}

void PrintCoord(const char *str, const COORDINATE_T &coord)
{
    printf("%s: %lf, %lf\n", str, coord.lng, coord.lat);
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

bool Test_Main()
{
    if (false == Test_CoordinateMapping()) {
        printf("Test_CoordinateMapping failed!\n");
    }
    if (false == Test_TileManager()) {
        printf("Test_TileManager failed!\n");
    }

    return true;
}
