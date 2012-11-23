#ifdef _WIN32
// Disable secuity warning message on MSVC
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <string.h>
#include <time.h>
#include "VehicleRecords.pb.h"
#include "VehicleRecords_Util.h"

#ifdef _WIN32
// Windows does not have snprintf, use _snprintf instead
#define snprintf _snprintf
#endif

using namespace std;
using namespace com::sap::nic::itrafic;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Local functions

static
void GetCurTimestamp(SQL_TIMESTAMP_STRUCT &st) {
    time_t t;
    time(&t);
    struct tm stm;
#ifdef _WIN32
    gmtime_s(&stm, &t);
#else
    localtime_r(&t, &stm);
#endif

    st.year = stm.tm_year + 1900;
    st.month = stm.tm_mon + 1;
    st.day = stm.tm_mday;
    st.hour = stm.tm_hour;
    st.minute = stm.tm_min;
    st.second = stm.tm_sec;
    st.fraction = 0;
}

static
unsigned long long TimestampToUint64(const SQL_TIMESTAMP_STRUCT &st) {
    unsigned long long u64 = 0;
    return u64;
}

static
void Uint64ToTimestamp(unsigned long long u64, SQL_TIMESTAMP_STRUCT &st) {
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// class VehicleRecord

VehicleRecords_Column::VehicleRecords_Column() {
    this->mCount = 0;
}

VehicleRecords_Column::~VehicleRecords_Column() {
}

void VehicleRecords_Column::GenerateRecords(int count) {
    this->mCount = count;
    this->ARR_GPSDATA_ID.clear();
    this->ARR_DEVID.clear();
    this->ARR_DEVID_LEN.clear();
    this->ARR_ALARMFLAG.clear();
    this->ARR_STATE.clear();
    this->ARR_STIME.clear();
    this->ARR_LATITUDE.clear();
    this->ARR_LONGTITUDE.clear();
    this->ARR_SPEED.clear();
    this->ARR_ORIENTATION.clear();
    this->ARR_GPSTIME.clear();
    this->ARR_ODOMETER.clear();
    this->ARR_OILGAUGE.clear();

    for (int i=0; i<count; i++) {
        {
            unsigned long long t = (unsigned long long)((double)rand()/RAND_MAX * 99999999);
            this->ARR_GPSDATA_ID.push_back(t);
        }
        {
            char buff[DEVID_LEN];
            memset(buff, 0, sizeof(buff));
            long t = (long)((double)rand()/RAND_MAX * 999999999);
            snprintf(buff, sizeof(buff), "%0ld", t);
            for (int k=0; k<DEVID_LEN; k++) {
                this->ARR_DEVID.push_back(buff[k]);
                this->ARR_DEVID_LEN.push_back(SQL_NTS);
            }
        }
        {
            SQL_TIMESTAMP_STRUCT st;
            GetCurTimestamp(st);
            st.minute = rand() % 60;
            st.second = rand() % 60;
            this->ARR_STIME.push_back(st);
        }
        {
            int t = (int)((double)rand()/RAND_MAX * 51200);
            this->ARR_ALARMFLAG.push_back(t);
        }
        {
            const static int STATES[] = {0, 1, 256, 513, 768, 17152, 15753};
            int index = rand() % (sizeof(STATES)/sizeof(int));
            this->ARR_STATE.push_back(STATES[index]);
        }
        {
            double t1 = (double)rand()/RAND_MAX * 10 + 40 + rand()/RAND_MAX;
            double t2 = (double)rand()/RAND_MAX * 10 + 120 + rand()/RAND_MAX;
            this->ARR_LATITUDE.push_back(t1);
            this->ARR_LONGTITUDE.push_back(t2);
        }
        {
            int t = (int)((double)rand()/RAND_MAX * 1000 - 500);
            if (t < 0) t=0;
            this->ARR_SPEED.push_back(t);
        }
        {
            int t = (int)((double)rand()/RAND_MAX * 360 - 180);
            this->ARR_ORIENTATION.push_back(t);
        }
        {
            SQL_TIMESTAMP_STRUCT st;
            GetCurTimestamp(st);
            st.minute = rand() % 60;
            st.second = rand() % 60;
            this->ARR_GPSTIME.push_back(st);
        }
        {
            // ODOMETER and OILGAUGE are all NULLs, what to insert?
            this->ARR_ODOMETER.push_back(0);
            this->ARR_OILGAUGE.push_back(0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// class VehicleRecordUtil

void VehicleRecordUtil::ToProtoBuf(const VehicleRecords_Column &vr,
    com::sap::nic::itrafic::VehicleReports &pvr) {

    for (int i=0; i<vr.mCount; i++) {
        Report *pReport = pvr.add_report();
        pReport->set_gpsdata_id(vr.ARR_GPSDATA_ID[i]);
        // for devid
        {
            char devid[DEVID_LEN + 1];
            devid[DEVID_LEN] = '\0';
            if (vr.ARR_DEVID_LEN[i] == SQL_NTS) {
                strncpy(devid, vr.ARR_DEVID.data() + i * DEVID_LEN, sizeof(devid));
            } else {
                memcpy(devid, vr.ARR_DEVID.data() + i * DEVID_LEN, DEVID_LEN);
            }
            pReport->set_devid(devid);
        }
        pReport->set_stime(TimestampToUint64(vr.ARR_STIME[i]));
        pReport->set_alarmflag(vr.ARR_ALARMFLAG[i]);
        pReport->set_state(vr.ARR_STATE[i]);
        pReport->set_latitude(vr.ARR_LATITUDE[i]);
        pReport->set_longtitude(vr.ARR_LONGTITUDE[i]);
        pReport->set_speed(vr.ARR_SPEED[i]);
        pReport->set_orientation(vr.ARR_ORIENTATION[i]);
        pReport->set_gpstime(TimestampToUint64(vr.ARR_GPSTIME[i]));
        pReport->set_odometer(vr.ARR_ODOMETER[i]);
        pReport->set_oilgauge(vr.ARR_OILGAUGE[i]);
    }
}
