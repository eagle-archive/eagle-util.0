#include "afxwin.h"
#include <iostream>
#include <fstream>
#include <string>
#include "VehicleRecords.pb.h"
#include "VehicleRecords_Col.h"

using namespace std;
using namespace com::sap::nic::itraffic;

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

#include "Utils\Net\GenericHTTPClient.h"

const TCHAR SERVER[] = _T("192.168.1.27");
const INTERNET_PORT PORT = 8000; // INTERNET_DEFAULT_HTTP_PORT

int Test_Get()
{
    GenericHTTPClient httpRequest;
    PBYTE szHead, szHTML;
    DWORD dwHtmlLen = 0;

    if(httpRequest.Request(_T("http://192.168.1.27:8000/hello"))){
        szHead = httpRequest.QueryHTTPResponseHeader();
        szHTML = httpRequest.QueryHTTPResponse(dwHtmlLen);
    }

    return 0;
}

int Test_Post()
{


    return 0;
}

int Test_HttpPostWithVehicleRecords()
{
    Test_Get();
    return 0;
}
