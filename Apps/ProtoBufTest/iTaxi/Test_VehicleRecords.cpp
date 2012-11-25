#include <iostream>
#include <fstream>
#include <string>
#include "VehicleRecords.pb.h"
#include "VehicleRecords_Col.h"

using namespace std;
using namespace com::sap::nic::itrafic;

int Test_VehicleRecords() {
    int nCase = 1;
    printf("%d: verify the convertion from auto-generated records to proto buffer\n", nCase++);
    {
        VehicleRecords_Col vr;
        vr.GenerateRecords(2);
        VehicleRecords_Col vr2(vr);

        VehicleReports pvr, pvr2;
        vr.ToProtoBuf(pvr);
        vr2.ToProtoBuf(pvr2);

        string s1 = pvr.DebugString();
        string s2 = pvr2.DebugString();
        assert(s1 == s2);
        printf(s1.c_str());
    }

    printf("%d: verify convertion from auto-generated proto buffer records to VehicleRecords_Col\n", nCase++);
    {
        VehicleRecords_Col t, vr;
        t.GenerateRecords(2);
        VehicleReports pvr, pvr2;
        t.ToProtoBuf(pvr);
        t.Clear();

        vr.FromProtoBuf(pvr);
        vr.ToProtoBuf(pvr2);
        string s1 = pvr.DebugString();
        string s2 = pvr2.DebugString();
        printf(s1.c_str()); printf(s2.c_str());
        assert(s1 == s2);
    }

    return 0;
}
