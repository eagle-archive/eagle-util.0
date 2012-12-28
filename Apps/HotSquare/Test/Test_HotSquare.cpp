
#include "../HotSquare.h"

using namespace std;


bool Test_CoordinateMapping()
{
    COORDINATE_T coord, coord2;
    coord.lat = 0.000500;
    coord.lng = 0.000500;
    unsigned long long id = CoordinateToSquareId(&coord);
    SquareIdToCoordinate(id, &coord2);
    return true;
}

bool Test_Main()
{
    if (false == Test_CoordinateMapping()) {
        printf("Test_CoordinateMapping failed!");
    }

    return true;
}
