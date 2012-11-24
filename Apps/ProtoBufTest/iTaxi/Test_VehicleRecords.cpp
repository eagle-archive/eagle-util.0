#include <iostream>
#include <fstream>
#include <string>
#include "VehicleRecords.pb.h"
#include "VehicleRecords_Util.h"

using namespace std;
using namespace com::sap::nic::itrafic;

int Test_VehicleRecords() {
    // verify the auto-generated records convertion
    {
        VehicleRecords_Column vr;
        vr.GenerateRecords(2);
        VehicleRecords_Column vr2(vr);

        VehicleReports pvr, pvr2;
        vr.ToProtoBuf(pvr);
        vr2.ToProtoBuf(pvr2);

        string s1 = pvr.DebugString();
        string s2 = pvr2.DebugString();
        assert(s1 == s2);
        printf(s1.c_str());
    }
    return 0;
}
