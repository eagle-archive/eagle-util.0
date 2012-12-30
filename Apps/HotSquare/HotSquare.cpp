// HotSquare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <hash_set>
#include "HotSquare.h"


using namespace std;

extern bool Test_Main();


int main()
{
    SegManager segManager;
    TileManager tileManager;

    if (false == segManager.LoadFromCsvFile(SEGMENTS_CSV_PATH))
    {
        printf("Error: cannot read Segments CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 10;
    }
    printf("%s: Found %d segments.\n",
        ElapsedTimeStr().c_str(), segManager.GetSegArrayCount());

    if (false == tileManager.GenerateTiles(segManager)) {
        printf("Error: cannot generate tiles\n");
        return 20;
    }
    printf("%s: Generated %d tiles.\n",
        ElapsedTimeStr().c_str(), tileManager.GetTileCount());

    hash_set<SQUARE_ID_T> squareIdSet;
    CalculateSquareIds_Multi(segManager.GetSegArray(), segManager.GetSegArrayCount(), 4, squareIdSet);
    printf("%s: Generated %d squares.\n",
        ElapsedTimeStr().c_str(), squareIdSet.size());

    //Test_Main();
    printf("%s: Done!\n", ElapsedTimeStr().c_str());
	return 0;
}
