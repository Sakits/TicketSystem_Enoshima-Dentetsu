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
    int hour = 0;
    int minute = 0;
    HourMinute(){}
    HourMinute(string str) : hour((str[0]-'0')*10+str[1]-'0'), minute((str[3]-'0')*10+str[4]-'0'){}

    friend ostream &operator<<(ostream &os, const HourMinute &hourMinute) {
        os << ((hourMinute.hour < 10) ? "0" : "") << hourMinute.hour << ":" << ((hourMinute.minute < 10) ? "0" : "") << hourMinute.minute;
        return os;
    }
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
    operator int() {
        testvaild();
        int ans = date;
        int i = month;
        if (i == 9) {ans += 31; --i;}
        if (i == 8) {ans += 31; --i;}
        if (i == 7) {ans += 30; --i;}
        return ans;
    }
};

typedef cStringType<21> Username;
typedef cStringType<31> Password;
typedef cStringType<25> Name;
typedef cStringType<33> MailAddr;
typedef int Privilege;

struct User {
    Username username;
    Privilege privilege;
    Name name;
    MailAddr mailAddr;
    Password password;

    bool operator<(const User &rhs) const {
        return username < rhs.username;
    }
};
//这是已登录用户对用户操作时已登录用户只需要知道的数据
//如何保证修改的同步性？用哈希吗？
//比如修改了权限，那么不仅用户信息中的权限需要修改，已经登录用户的权限也需要修改。
//要能快速查找
//Username username;
//Privilege privilege;

//Name name;
//MailAddr mailAddr;

typedef cStringType<22> TrainID;
typedef int StationNum,SeatNum,TicketNum,OrderNumth;
typedef int TravelTime,StopoverTime;
typedef int Price;
typedef cStringType<44> Station;
typedef HourMinute StartTime;
typedef MonthDate SaleDate;
typedef cStringType<2> Type;
typedef cStringType<10> TwoChoice;
typedef string Stations,Prices,TravelTimes,StopoverTimes,SaleDates;

struct Train {
    static constexpr int STATIONNUMMAX = 101;
    TrainID trainId;
    StationNum stationNum;
    Station stations[STATIONNUMMAX];//这里可以用表示替代，注意！
    SeatNum seatNum;
    Price prices[STATIONNUMMAX];
    StartTime startTime;
    TravelTime travelTimes[STATIONNUMMAX];
    StopoverTime stopoverTimes[STATIONNUMMAX];
    SaleDate saleDate;
    Type type;
};

/*struct LoginUsers{//(这些或许可用指针，移动快？）
    adduser
    deleteuser
    finduser
}loginUsers;

struct NotOnSaleTrains{
    addtrain
    deleteTrain
}notOnSaleTrains;

struct OnSaleTrains{
    saleticket
    querytrain
}onSaleTrains;

    releaseTrain*/
#endif //TRAINTICKET_BASICHEADER_H
