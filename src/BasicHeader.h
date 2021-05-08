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
        return *this;
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

    friend std::ostream &operator<<(std::ostream &os, const cStringType &type) {
        os << type.c;
        return os;
    }

    template<class T>
    std::string operator+(T t) {
        return std::string(*this) + t;
    }

    friend std::string operator+(std::string s, cStringType<N> t) {
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
        str += ('0' + hour / 10);
        str += ('0' + (hour - hour / 10 * 10));
        str += ':';
        str += ('0' + minute / 10);
        str += ('0' + (minute - minute / 10 * 10));
        return str;
    }

    explicit operator int() const {
        return hour*24+minute;
    }

    int operator+=(int x) {
        minute += x;
        hour += minute / 60;
        minute %= 60;
        int overflow = hour / 24;
        hour %= 24;
        return overflow;//better 取模是否可优化？
    }

    bool operator<(const HourMinute &rhs) const {
        if (hour < rhs.hour)
            return true;
        if (rhs.hour < hour)
            return false;
        return minute < rhs.minute;
    }




    friend std::ostream &operator<<(std::ostream &os, const HourMinute &hourMinute) {
        os << ((hourMinute.hour < 10) ? "0" : "") << hourMinute.hour << ":" << ((hourMinute.minute < 10) ? "0" : "")
           << hourMinute.minute;
        return os;
    }
};

struct FullDate;

struct MonthDate {
    int month = 0;
    int date = 0;

    void testvaild() const {
        if (month < 6 || month > 8 || date < 0 || date > 31) {
            std::cout << std::string(*this) << std::endl;
//        log();
//        exit(0);
//        throw ("sss");
        }
    }

    static constexpr int calendar[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    MonthDate() {}

    MonthDate(int month, int date) : month(month), date(date) { testvaild(); }

    MonthDate(FullDate);

    MonthDate(std::string str) : month(str[1] - '0'), date((str[3] - '0') * 10 + str[4] - '0') {}

    MonthDate &operator++() {
        testvaild();
        if (date == calendar[month]) {
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


    explicit operator int() const {
        testvaild();
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
        str += ('0' + month / 10);
        str += ('0' + (month - month / 10 * 10));
        str += '-';
        str += ('0' + date / 10);
        str += ('0' + (date - date / 10 * 10));
        return str;
    }

    friend std::ostream &operator<<(std::ostream &os, const MonthDate &date) {
        os << std::string(date);
        return os;
    }
};

constexpr int MonthDate::calendar[13];

struct FullDate {
    MonthDate monthDate;
    HourMinute hourMinute;

    FullDate() {}

    FullDate(const MonthDate &monthDate, const HourMinute &hourMinute) : monthDate(monthDate), hourMinute(hourMinute) {}

    FullDate &operator+=(int x) {
//        if(x == -1){hourMinute.hour = -1, monthDate.month = 0; return *this;}
        monthDate += (hourMinute += x);
        return *this;
    }

    bool operator<(const FullDate &rhs) const {
        if (monthDate < rhs.monthDate)
            return true;
        if (rhs.monthDate < monthDate)
            return false;
        return hourMinute < rhs.hourMinute;
    }

    explicit operator int() const {
        return int(monthDate)*1440 + int(hourMinute);
    }

    operator std::string() const {
        return std::string(monthDate) + " " + std::string(hourMinute);
    }

    friend std::ostream &operator<<(std::ostream &os, const FullDate &date) {
        os << std::string(date);
        return os;
    }

//    operator MonthDate(){
//        return monthDate;
//    }

};

MonthDate::MonthDate(FullDate fullDate) {
    month = fullDate.monthDate.month;
    date = fullDate.monthDate.date;
}

typedef cStringType<21> Username;
typedef cStringType<31> Password;
typedef cStringType<25> Name;
typedef cStringType<33> MailAddr;
typedef int Privilege, OrderNumth;

struct User {
    Name name;
    MailAddr mailAddr;
    Password password;
    Privilege privilege;

    User() = default;

    User(Privilege privilege, const Name &name, const MailAddr &mailAddr,
         const Password &password, OrderNumth orderNumth = 0) : privilege(privilege), name(name), mailAddr(mailAddr),
                                                                password(password) {}

};//hack 坚持使用cStringType, rawuser 是否就不需要了？
//better existUsers可以全部丢进内存吗？

InnerUniqueUnorderMap<Username, Privilege, std::hash<std::string>> loginUsers;
//better 订单可以放在内存里，等logout或者exit时写回去吗？那样就要有通知补票的机制，怎么补呢？对于有保证login的操作，都可以如此做吗？
OuterUniqueUnorderMap<Username, User, std::hash<std::string>> existUsers("existUsers.dat","bpt_existUsers.dat");


typedef cStringType<22> TrainID;
typedef int StationNum, SeatNum, TicketNum;
typedef int PassedMinutes;
typedef int Price;
typedef cStringType<44> StationName;
typedef HourMinute StartTime;
typedef cStringType<2> Type;
typedef cStringType<10> TwoChoice;
typedef std::string StationNames, Prices, TravelTimes, StopoverTimes, SaleDates;

struct Train {
    static constexpr int STATIONMAX = 101;
    static constexpr int DAYMAX = 100;
    StationNum stationNum;
    StationName stations[STATIONMAX];//这里可以用表示替代，注意！
    SeatNum seatNum;
    Price prices[STATIONMAX];
    StartTime startTime;
    PassedMinutes arrivingTimes[STATIONMAX] = {0};
    PassedMinutes leavingTimes[STATIONMAX] = {0};//FIXME 应该是直接算出每站的绝对时间
    PassedMinutes duration;
    MonthDate startSaleDate, endSaleDate;
    Type type;
    TicketNum ticketNums[DAYMAX][STATIONMAX] = {0};
    bool is_released = false;

    Train(){}

    //caution 拷贝赋值都是浅拷贝！
//    Train(const Train&) = delete;
//    Train &operator=(const Train&) = delete;
    Train(StationNum _stationNum, const sjtu::vector<StationName> &_stations, SeatNum _seatNum,
          const sjtu::vector<Price> &_prices, const StartTime &_startTime,
          const sjtu::vector<PassedMinutes> &_arrivingTimes, const sjtu::vector<PassedMinutes> &_leavingTimes,
          const sjtu::vector<MonthDate> &_saleDate, const Type &_type) : stationNum(_stationNum), seatNum(_seatNum),
                                                                         startTime(_startTime), type(_type) {
        for (int i = 0; i < stationNum; ++i) stations[i] = _stations[i];
        arrivingTimes[0] = leavingTimes[stationNum - 1] = 0;
        arrivingTimes[1] = _arrivingTimes[0], leavingTimes[0] = 0;
        prices[0] = 0;
        for (int i = 1; i < stationNum; ++i) {
            prices[i] = prices[i - 1] + _prices[i - 1];
        }
        for (int i = 1; i <= stationNum - 2; ++i) {
            leavingTimes[i] = arrivingTimes[i] + _leavingTimes[i - 1];
            arrivingTimes[i + 1] = leavingTimes[i] + _arrivingTimes[i];
        }
        startSaleDate = _saleDate[0], endSaleDate = _saleDate[1];

        duration = int(_saleDate[1]) - int(_saleDate[0]) + 1;
        for (int i = 0; i < duration; ++i)
            for (int j = 0; j < _stationNum - 1; ++j)
                ticketNums[i][j] = _seatNum;
    }

    int findStation(const StationName &stationName) const {
        for (int i = 0; i < stationNum; ++i) {
            if (stations[i] == stationName) return i;
        }
        return -1;
    }
};


OuterUniqueUnorderMap<TrainID, Train, std::hash<std::string>> existTrains("existTrains.dat", "bpt_existTrains.dat");

typedef cStringType<10> Status;

struct Order {
    Username username;//username其实并不需要，不过先留着吧。
    Status status;

    TrainID trainID;
    StationName fromStation;
    StationName toStation;
    FullDate arrivingTime;
    FullDate leavingTime;
    Price price;
    int num;
    //hack 时间戳
    static int timestamp;
    int timestamplocal;

    //或许需要ordernumth来做唯一认证，在refundticket的时候可以知道退了炸队列的到底是队列里的哪笔订单？
    Order() {}

    Order(const Username &username, const Status &status, const TrainID &trainId,
          const StationName &fromStation, const StationName &toStation,
          const FullDate &leavingTime, const FullDate &arrivingTime, Price price, int num) : username(username),
                                                                                             status(status),
                                                                                             trainID(trainId),
                                                                                             fromStation(fromStation),
                                                                                             toStation(toStation),
                                                                                             leavingTime(leavingTime),
                                                                                             arrivingTime(arrivingTime),
                                                                                             price(price),
                                                                                             num(num) { timestamplocal = ++timestamp; }

    operator std::string() {
        std::string ans;
        ans += (trainID + " " + fromStation + " " + std::string(leavingTime) + " -> " + toStation + " " +
                std::string(arrivingTime) + " " + std::to_string(price) + " " + std::to_string(num));
        return ans;
    }

    bool operator==(const Order &rhs) const {
        return
                timestamplocal == rhs.timestamplocal &&//hack
                username == rhs.username &&
                trainID == rhs.trainID &&
                fromStation == rhs.fromStation &&
                toStation == rhs.toStation;
    }

    bool operator!=(const Order &rhs) const {
        return !(rhs == *this);
    }
};

int Order::timestamp = 0;
//stub
#include <set>

std::unordered_map<StationName, std::set<TrainID>, std::hash<std::string>> stmap;

void addPassedTrain(StationName stationName, TrainID trainID) {
    //stub
    stmap[stationName].insert(trainID);
}

std::set<TrainID> findCommonTrain(StationName fromStation, StationName toStation) {
    //stub
    auto iter_s = stmap.find(fromStation);
    if (iter_s == stmap.end()) return std::set<TrainID>();
    auto iter_t = stmap.find(toStation);
    if (iter_t == stmap.end()) return std::set<TrainID>();
    auto it_i = iter_s->second;
    auto it_j = iter_t->second;
    auto j = it_j.begin();
    std::set<TrainID> ret;
    for (auto i = it_i.begin(); i != it_i.end(); ++i) {
        for (; *j < *i; ++j)
            if (j == it_j.end()) goto BREAK;
        if (*j == *i) ret.insert(*j);
    }
    BREAK:
    return ret;
}

struct midStation{
    StationName stationName;
    sjtu::vector<TrainID> start;
    sjtu::vector<TrainID> end;
};
std::set<midStation> findMidStation(StationName fromStation, StationName toStation){

}

InnerOuterMultiUnorderMap<Username, Order, std::hash<std::string>> userOrders;
//FIXME 对这个类的使用是不正确的。。。。。。要求是，能快速访问userOrders.

//typedef OuterUniqueUnorderMap<TrainID, Train, std::hash<std::string>>::Iterator TrainPtr;


Queue<Order> waitQueue("wait_queue.dat");


#endif //TRAINTICKET_BASICHEADER_H
