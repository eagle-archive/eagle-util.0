#ifndef __VEHICLERECORDS_UTIL_H_
#define __VEHICLERECORDS_UTIL_H_

#include <vector>
#include <string>
#if defined(WIN32)
#include <windows.h> // required by sqlext.h for WIN32
#endif
#include "sqlext.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
create column table "ITRAFFIC"."PROTO_ODBC_SINGLE"(
    "GPSDATA_ID" DECIMAL (20) null,
    "DEVID" VARCHAR (20) null,
    "STIME" TIMESTAMP null,
    "ALARMFLAG" DECIMAL (6) null,
    "STATE" DECIMAL (6) null,
    "LATITUDE" DOUBLE null,
    "LONGTITUDE" DOUBLE null,
    "SPEED" INTEGER null,
    "ORIENTATION" DOUBLE null,
    "GPSTIME" TIMESTAMP null,
    "ODOMETER" DOUBLE null,
    "OILGAUGE" DOUBLE null)
*/

#define GPSDATA_ID_LEN  20
#define DEVID_LEN       20

/*
    Class for vehicle records in cloumn mode
*/
class VehicleRecords_Column {
public:
    VehicleRecords_Column() {
        mCount = 0;
    };
    VehicleRecords_Column(const VehicleRecords_Column& from) {
        CopyFrom(from);
    };
    virtual ~VehicleRecords_Column() {};

public:
    void Clear();
    void CopyFrom(const VehicleRecords_Column& from);
    void GenerateRecords(int count);
    void ToProtoBuf(com::sap::nic::itrafic::VehicleReports &pvr);
    void FromProtoBuf(const com::sap::nic::itrafic::VehicleReports &pvr);

protected:
    int mCount;

    std::vector<SQLUBIGINT>             ARR_GPSDATA_ID;
    std::vector<char>                   ARR_DEVID;
    std::vector<SQLLEN>                 ARR_DEVID_LEN; // for length of ARR_DEVID
    std::vector<SQLUINTEGER>            ARR_ALARMFLAG;
    std::vector<SQLUINTEGER>            ARR_STATE;
    std::vector<SQL_TIMESTAMP_STRUCT>   ARR_STIME;
    std::vector<SQLDOUBLE>              ARR_LATITUDE;
    std::vector<SQLDOUBLE>              ARR_LONGTITUDE;
    std::vector<SQLINTEGER>             ARR_SPEED;
    std::vector<SQLDOUBLE>              ARR_ORIENTATION;
    std::vector<SQL_TIMESTAMP_STRUCT>   ARR_GPSTIME;
    std::vector<SQLDOUBLE>              ARR_ODOMETER;
    std::vector<SQLDOUBLE>              ARR_OILGAUGE;
};

#ifdef __cplusplus
}
#endif
#endif // __VEHICLERECORDS_UTIL_H_
