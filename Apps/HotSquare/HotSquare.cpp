// HotSquare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <hash_set>
#include "HotSquare.h"
#include "SquareManager.h"


using namespace std;

extern bool Test_Main();

SegManager gSegManager;
TileManager gTileManager;
SquareManager gSquareManager;

int main()
{
    if (false == gSegManager.LoadFromCsvFile(SEGMENTS_CSV_PATH))
    {
        printf("Error: cannot read Segments CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 10;
    }
    printf("%s: Found %d segments.\n",
        ElapsedTimeStr().c_str(), gSegManager.GetSegArrayCount());

    if (false == gTileManager.GenerateTiles(gSegManager)) {
        printf("Error: cannot generate tiles\n");
        return 20;
    }
    printf("%s: Generated %d tiles.\n",
        ElapsedTimeStr().c_str(), gTileManager.GetTileCount());
    gTileManager.SaveToCsvFile("Data\\Tiles.txt");

    Test_Main();

    gSquareManager.BuildSquareMap_Multi(gSegManager, 4);
    printf("%s: Generated %d squares.\n", ElapsedTimeStr().c_str(), gSquareManager.GetSquareCount());

    printf("%s: Done!\n", ElapsedTimeStr().c_str());
	return 0;
}
