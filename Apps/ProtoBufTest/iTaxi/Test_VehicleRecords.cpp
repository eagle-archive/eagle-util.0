#include "afxwin.h"
#include <iostream>
#include <fstream>
#include <string>
#include "VehicleRecords.pb.h"
#include "VehicleRecords_Col.h"

using namespace std;
using namespace com::sap::nic::itraffic;

static void GenerateProtoBuff(VehicleReports &vr, int num) {
    VehicleRecords_Col vrc;
    vrc.GenerateRecords(num);
    vrc.ToProtoBuf(vr);
}

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

const char URL[] = "http://192.168.1.27:8000/hello";

int Test_Get()
{
    GenericHTTPClient httpRequest;
    std::string szHead, szHTML;

    if (httpRequest.Request(URL)) {
        szHead = httpRequest.QueryHTTPResponseHeader();
        szHTML = httpRequest.QueryHTTPResponse();
    }
    httpRequest.Close();
    return 0;
}

int Test_Post()
{
    GenericHTTPClient *pClient=new GenericHTTPClient();
    string szResult;
    DWORD dwRseLen = 0;

    VehicleReports vr;
    GenerateProtoBuff(vr, 3);
    //printf(vr.DebugString().c_str());
    string strVR = vr.SerializeAsString();

    pClient->InitilizePostArguments();
    pClient->AddPostArguments("VehicleReports", (PBYTE)strVR.c_str(), strVR.size(), TRUE);

    if (pClient->Request(URL, GenericHTTPClient::RequestPostMethod)) {
        szResult = pClient->QueryHTTPResponse();
    }
    pClient->Close();

    delete pClient;

    return 0;
}

int Test_HttpPostWithVehicleRecords()
{
    //Test_Get();
    Test_Post();
    return 0;
}
