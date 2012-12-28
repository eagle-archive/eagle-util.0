// HotSquare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HotSquare.h"




using namespace std;

extern bool Test_Main();

int main()
{
    vector<SEGMENT_T> segments;

    if (false == ReadFromCsv(SEGMENTS_CSV_PATH, segments))
    {
        printf("Error: cannot read Segments CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 1;
    }

    Test_Main();

	return 0;
}
