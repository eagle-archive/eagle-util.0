// HotSquare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HotSquare.h"




using namespace std;

int main()
{
    vector<SEGMENT_T> segments;

    if (false == ReadFromCSV(SEGMENTS_CSV_PATH, segments))
    {
        printf("Error: cannot read CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 1;
    }



	return 0;
}
