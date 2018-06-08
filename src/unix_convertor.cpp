#include "unix_convertor.h"
#include <stdio.h>

namespace my_plugin {

char* time::toString(){
    char* buf = new char[100];
    sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d",this->nYear, this->nMonth, this->nDay,
            this->nHour, this->nMin, this->nSec);
    return buf;
}

bool time::readTimeFromString(time& time,char* timeStr){
    int res = sscanf(timeStr,"%04u-%02u-%02u %02u:%02u:%02u",&time.nYear,&time.nMonth,&time.nDay,&time.nHour,&time.nMin,&time.nSec);
    return res>=6;
}


const unsigned char unix_convertor::g_day_per_mon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned char unix_convertor::applib_dt_is_leap_year(unsigned short year){
    if ((year % 400) == 0) {
        return 1;
    } else if ((year % 100) == 0) {
        return 0;
    } else if ((year % 4) == 0) {
        return 1;
    } else {
        return 0;
    }
}

unsigned char unix_convertor::applib_dt_last_day_of_mon(unsigned char month, unsigned short year){
    if ((month == 0) || (month > 12)) {
        return g_day_per_mon[1] + applib_dt_is_leap_year(year);
    }

    if (month != 2) {
        return g_day_per_mon[month - 1];
    } else {
        return g_day_per_mon[1] + applib_dt_is_leap_year(year);
    }
}



void unix_convertor::utc_sec_2_time(unsigned long utc_sec, time& result, bool daylightSaving){
    int sec, day;
    unsigned short y;
    unsigned char m;
    unsigned short d;
    if (daylightSaving) {
        utc_sec += SEC_PER_HOUR;
    }

    /* hour, min, sec */
    /* hour */
    sec = utc_sec % SEC_PER_DAY;
    result.nHour = sec / SEC_PER_HOUR;

    /* min */
    sec %= SEC_PER_HOUR;
    result.nMin = sec / SEC_PER_MIN;

    /* sec */
    result.nSec = sec % SEC_PER_MIN;

    /* year, month, day */
    /* year */
    /* year */
    day = utc_sec / SEC_PER_DAY;
    for (y = UTC_BASE_YEAR; day > 0; y++) {
        d = (DAY_PER_YEAR + applib_dt_is_leap_year(y));
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }

    result.nYear = y;

    for (m = 1; m < MONTH_PER_YEAR; m++) {
        d = applib_dt_last_day_of_mon(m, y);
        if (day >= d) {
            day -= d;
        } else {
            break;
        }
    }

    result.nMonth = m;
    result.nDay = (unsigned char) (day + 1);
}


unsigned long unix_convertor::time_2_utc_sec(const time& currTime, bool daylightSaving){
    unsigned short i;
    unsigned int no_of_days = 0;
    unsigned long utc_time;
    if (currTime.nYear < UTC_BASE_YEAR) {
        return 0;
    }

    /* year */
    for (i = UTC_BASE_YEAR; i < currTime.nYear; i++) {
        no_of_days += (DAY_PER_YEAR + applib_dt_is_leap_year(i));
    }

    /* month */
    for (i = 1; i < currTime.nMonth; i++) {
        no_of_days += applib_dt_last_day_of_mon((unsigned char) i, currTime.nYear);
    }

    /* day */
    no_of_days += (currTime.nDay - 1);

    /* sec */
    utc_time = (unsigned long) no_of_days * SEC_PER_DAY + (unsigned long) (currTime.nHour * SEC_PER_HOUR +
                                                                           currTime.nMin * SEC_PER_MIN + currTime.nSec);

    if (daylightSaving) {
        utc_time -= SEC_PER_HOUR;
    }

    return utc_time;
}


}
