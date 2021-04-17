//
// Created by JiamingShan on 2021/4/16.
//

#ifndef TRAINTICKET_BASICHEADER_H
#define TRAINTICKET_BASICHEADER_H


#include <regex>
#include <cstring>
#include <string>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <utility>

using namespace std;

template<int N>
class cStringType {
private:
    char c[N] = {0};
    size_t sz = 0;
public:
    cStringType() {}

    cStringType(const char *_c) {
        sz = strlen(_c);
        for (size_t i = 0; i < sz; ++i) {
            c[i] = _c[i];
        }
    }

    cStringType(const string &_c) {
        sz = _c.size();
        for (size_t i = 0; i < sz; ++i) {
            c[i] = _c[i];
        }
    }

    cStringType &operator=(const char *_c) {
        memset(c, 0, sizeof(c));
        sz = strlen(_c);
        for (size_t i = 0; i < sz; ++i) {
            c[i] = _c[i];
        }
    }

    operator string() const {
        return c;
    }

    size_t size() const {
        return sz;
    }

    bool operator==(const cStringType &rhs) const {
        return string(c) == string(rhs.c);
    }

    bool operator!=(const cStringType &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const cStringType &rhs) const {
        return string(c) < string(rhs.c);
    }

    bool operator>(const cStringType &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const cStringType &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const cStringType &rhs) const {
        return !(*this < rhs);
    }

    friend ostream &operator<<(ostream &os, const cStringType &type) {
        os << type.c;
        return os;
    }
};

struct HourMinute {

};
struct MonthDate {
    int month = 6;
    int date = 1;
    void testvaild(){assert(month < 6 || month > 8 || date < 0 || date > 31);}
    MonthDate(int month, int date) : month(month), date(date) {testvaild();}
    MonthDate(string str) : month(str[1] - '0'), date((str[3]-'0')*10+str[4]-'0') {}
    MonthDate& operator++(){
        testvaild();
        if((date == 30 && month == 6) || (date == 31 && (month == 7 || month == 8))){
            month += 1, date = 1;
            return *this;
        }
        date += 1;
        return *this;
    }
    operator int(){
        testvaild();

    }
};

typedef cStringType<21> Username;
typedef cStringType<31> Password;
typedef cStringType<25> Name;
typedef cStringType<33> MailAddr;
typedef int Privilege;

struct User {
    Username username;
    Password password;
    Name name;
    MailAddr mailAddr;
    Privilege privilege;

    bool operator<(const User &rhs) const {
        return username < rhs.username;
    }
};

typedef cStringType<22> TrainID;
typedef int StationNum;
typedef cStringType<44> Station;
typedef int SeatNum;
typedef int Price;
typedef HourMinute StartTime;
typedef int TravelTime;
typedef int StopoverTime;
typedef MonthDate SaleDate;
typedef char Type;

struct Train {
    static constexpr int stationNumMax = 101;
    TrainID trainId;
    StationNum stationNum;
    Station stations[stationNumMax];//这里可以用表示替代，注意！
    SeatNum seatNum;
    Price prices[stationNumMax];
    StartTime startTime;
    TravelTime travelTimes[stationNumMax];
    StopoverTime stopoverTimes[stationNumMax];
    SaleDate saleDate;
    Type type;
};


#endif //TRAINTICKET_BASICHEADER_H
