#ifndef UNIX_CONVERTOR_H
#define UNIX_CONVERTOR_H

#define UTC_BASE_YEAR 1970
#define MONTH_PER_YEAR 12
#define DAY_PER_YEAR 365
#define SEC_PER_DAY 86400
#define SEC_PER_HOUR 3600
#define SEC_PER_MIN 60

namespace my_plugin {
class time{
public:
    unsigned short nYear;
    unsigned char nMonth;
    unsigned char nDay;
    unsigned char nHour;
    unsigned char nMin;
    unsigned char nSec;
    void toString(char* buf);
    static bool readTimeFromString(time& time,char* timeStr);
};


class unix_convertor{

public:
    unix_convertor(){}
    void utc_sec_2_time(unsigned long utc_sec, time& result, bool daylightSaving);
    unsigned long time_2_utc_sec(const time& currTime, bool daylightSaving);
private:
    static const unsigned char g_day_per_mon[];
    unsigned char applib_dt_is_leap_year(unsigned short year);
    unsigned char applib_dt_last_day_of_mon(unsigned char month, unsigned short year);
};

}



#endif // UNIX_CONVERTOR_H
