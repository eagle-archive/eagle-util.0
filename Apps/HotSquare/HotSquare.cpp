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
    vector<SEGMENT_T> segments;
    hash_set<SQUARE_ID_T> squareIdSet;

    if (false == ReadFromCsv(SEGMENTS_CSV_PATH, segments))
    {
        printf("Error: cannot read Segments CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 1;
    }
    printf("%s: Found %d segments.\n",
        ElapsedTimeStr().c_str(), segments.size());

    CalculateSquareIds_Multi(segments, 4, squareIdSet);
    printf("%s: Generated %d squares.\n",
        ElapsedTimeStr().c_str(), squareIdSet.size());

    //Test_Main();
    printf("%s: Done!\n", ElapsedTimeStr().c_str());
	return 0;
}
