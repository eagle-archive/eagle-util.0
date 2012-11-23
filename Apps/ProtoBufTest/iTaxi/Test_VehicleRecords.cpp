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

        VehicleReports pvr;
        VehicleRecordUtil::ToProtoBuf(vr, pvr);
    }
    return 0;
}
