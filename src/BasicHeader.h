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
#include <unordered_map>
#include "vector.hpp"
#include "maps.h"
#include <vector>
//FIXME 这只是个stub，记得删了



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

    cStringType(const std::string &_c) {
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

    operator std::string() const {
        return c;
    }

//    operator char*() const{
//        return c;
//    }

    operator bool() const {
        return c == "";
    }

    size_t size() const {
        return sz;
    }

    bool operator==(const cStringType &rhs) const {
        return std::string(c) == std::string(rhs.c);
    }

    bool operator!=(const cStringType &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const cStringType &rhs) const {
        return std::string(c) < std::string(rhs.c);
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

    template<class T>
    std::string operator+(T t) {
        return std::string(*this) + t;
    }

    friend std::string operator+(string s, cStringType<N> t) {
        return s + std::string(t);
    }
};

struct HourMinute {
    int hour = -1;
    int minute = -1;

    HourMinute() {}

    HourMinute(std::string str) : hour((str[0] - '0') * 10 + str[1] - '0'),
                                  minute((str[3] - '0') * 10 + str[4] - '0') {}

    operator std::string() const {
        if (hour == -1) return "xx:xx";
        std::string str;
        if (hour < 10)str += '0';
        str += ('0' + hour);
        str += ':';
        if (minute < 10) str += '0';
        str += ('0' + minute);
        return str;
    }

    int operator+=(int x) {
        minute += x;
        hour += minute / 60;
        minute %= 60;
        int overflow = hour / 24;
        hour %= 24;
        return overflow;//better 取模是否可优化？
    }

    friend ostream &operator<<(ostream &os, const HourMinute &hourMinute) {
        os << ((hourMinute.hour < 10) ? "0" : "") << hourMinute.hour << ":" << ((hourMinute.minute < 10) ? "0" : "")
           << hourMinute.minute;
        return os;
    }
};

struct MonthDate {
    int month = 0;
    int date = 0;

    void testvaild() const { assert(month < 6 || month > 8 || date < 0 || date > 31); }

    MonthDate() {}

    MonthDate(int month, int date) : month(month), date(date) { testvaild(); }

    MonthDate(std::string str) : month(str[1] - '0'), date((str[3] - '0') * 10 + str[4] - '0') {}

    MonthDate &operator++() {
        testvaild();
        if ((date == 30 && month == 6) || (date == 31 && (month == 7 || month == 8))) {
            month += 1, date = 1;
            return *this;
        }
        date += 1;
        return *this;
    }

    MonthDate &operator+=(int x) {
        while (x--)++*this;
        return *this;
    }

    operator int() const {
        testvaild();
        const int calendar[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int ans = date;
        for (int i = month; i != 6; --i)
            ans += calendar[i - 1];
        return ans;
    }

    bool operator<(const MonthDate &rhs) const {
        if (month < rhs.month)
            return true;
        if (rhs.month < month)
            return false;
        return date < rhs.date;
    }

    bool operator<=(const MonthDate &rhs) const {
        return !(rhs < *this);
    }

    operator std::string() const {
        if (month == 0) return "xx-xx";
        std::string str;
        str += '0';
        str += ('0' + month);
        str += '-';
        if (date < 10) str += '0';
        str += ('0' + date);
        return str;
    }

    friend ostream &operator<<(ostream &os, const MonthDate &date) {
        os << std::string(date);
        return os;
    }
};

struct FullDate {
    MonthDate monthDate;
    HourMinute hourMinute;

    FullDate(const MonthDate &monthDate, const HourMinute &hourMinute) : monthDate(monthDate), hourMinute(hourMinute) {}

    FullDate &operator+=(int x) {
        monthDate += (hourMinute += x);
        return *this;
    }


    operator std::string() const {
        return std::string(monthDate) + " " + std::string(hourMinute);
    }

    friend ostream &operator<<(ostream &os, const FullDate &date) {
        os << std::string(date);
        return os;
    }

};

typedef cStringType<21> Username;
typedef cStringType<31> Password;
typedef cStringType<25> Name;
typedef cStringType<33> MailAddr;
typedef int Privilege;

struct User;


struct User {
    Name name;
    MailAddr mailAddr;
    Password password;
    Privilege privilege;

    User() = default;

    User(Privilege privilege, const Name &name, const MailAddr &mailAddr,
         const Password &password) : privilege(privilege), name(name), mailAddr(mailAddr),
                                     password(password) {}

    operator int() {
        return privilege;
    }

};//hack 坚持使用cStringType, rawuser 是否就不需要了？

InnerUniqueUnorderMap<Username, Privilege, std::hash<std::string>> loginUsers;
OuterUniqueUnorderMap<Username, User, std::hash<std::string>> existUsers;


typedef cStringType<22> TrainID;
typedef int StationNum, SeatNum, TicketNum, OrderNumth;
typedef int ArrivingTime, LeavingTime;
typedef int Price;
typedef cStringType<44> StationName;
typedef HourMinute StartTime;
typedef MonthDate SaleDate;
typedef cStringType<2> Type;
typedef cStringType<10> TwoChoice;
typedef std::string StationNames, Prices, TravelTimes, StopoverTimes, SaleDates;

struct Train {
    static constexpr int STATIONMAX = 101;
    static constexpr int DAYMAX = 65;
    StationNum stationNum;
    StationName stations[STATIONMAX];//这里可以用表示替代，注意！
    SeatNum seatNum;
    Price priceSumSinceStart[STATIONMAX];
    StartTime startTime;
    ArrivingTime arrivingTimes[STATIONMAX] = {0};
    LeavingTime leavingTimes[STATIONMAX] = {0};//FIXME 应该是直接算出每站的绝对时间
    SaleDate startSaleDate, endSaleDate;
    Type type;
    TicketNum ticketNums[DAYMAX][STATIONMAX] = {0};
    bool is_released = false;

    Train(StationNum _stationNum, const sjtu::vector<StationName> &_stations, SeatNum _seatNum,
          const sjtu::vector<Price> &_prices, const StartTime &_startTime,
          const sjtu::vector<ArrivingTime> &_arrivingTimes, const sjtu::vector<LeavingTime> &_leavingTimes,
          const sjtu::vector<SaleDate> &_saleDate, const Type &_type) : stationNum(_stationNum), seatNum(_seatNum),
                                                                        startTime(_startTime), type(_type) {
        for (int i = 0; i < stationNum; ++i) stations[i] = _stations[i];
        arrivingTimes[0] = leavingTimes[stationNum - 1] = -1;
        arrivingTimes[1] = _arrivingTimes[0], leavingTimes[0] = 0;
        priceSumSinceStart[0] = 0;
        for (int i = 1; i < stationNum; ++i) {
            priceSumSinceStart[i] = priceSumSinceStart[i - 1] + _prices[i];
        }
        for (int i = 1; i <= stationNum - 2; ++i) {
            leavingTimes[i] = arrivingTimes[i] + _leavingTimes[i - 1];
            arrivingTimes[i + 1] = leavingTimes[i] + _arrivingTimes[i];
        }
        startSaleDate = _saleDate[0], endSaleDate = _saleDate[1];

    }
};


OuterUniqueUnorderMap<TrainID, Train, std::hash<string>> existTrains;
typedef OuterUniqueUnorderMap<TrainID, Train, std::hash<string>>::Iterator TrainPtr;

struct Tickets {

};


struct UserOrders {
    void buy_ticket() {}

    void refund_ticket() {}

    void query_order() {}
};

struct WaitingQueue {

};

//    releaseTrain
#endif //TRAINTICKET_BASICHEADER_H
