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
#include "cassert"
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
        return overflow;
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
        int ans = date;
        int i = month;
        if (i == 9) {
            ans += 31;
            --i;
        }
        if (i == 8) {
            ans += 31;
            --i;
        }
        if (i == 7) {
            ans += 30;
            --i;
        }
        return ans;
    }

    bool operator<(const MonthDate &rhs) const {
        if (month < rhs.month)
            return true;
        if (rhs.month < month)
            return false;
        return date < rhs.date;
    }

    bool operator>(const MonthDate &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const MonthDate &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const MonthDate &rhs) const {
        return !(*this < rhs);
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

struct RawUser {
    char mailAddr[31];
    char password[31];
    char name[16];
    int privilege;

    RawUser(std::string _mailAddr, std::string _password, std::string _name, int privilege) {
        memcpy(mailAddr, _mailAddr.c_str(), 31);
        memcpy(password, _password.c_str(), 31);
        memcpy(name, _name.c_str(), 16);
    }

    User wrap();
};

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

    RawUser unwrap() {
        return RawUser{name, mailAddr, password, privilege};
    }
};

User RawUser::wrap() {
    return User{privilege, name, mailAddr, password};
}
//这是已登录用户对用户操作时已登录用户只需要知道的数据
//如何保证修改的同步性？用哈希吗？
//比如修改了权限，那么不仅用户信息中的权限需要修改，已经登录用户的权限也需要修改。
//要能快速查找
//Username username;
//Privilege privilege;

//Name name;
//MailAddr mailAddr;

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


template<class Key, class Value, class Hash>
class OuterUniqueUnorderMap {
public:

    bool insert(const std::pair<Key, Value> &pair) {
        return mapper.insert(pair).second;
    }

    bool erase(const Key &key) {
        return mapper.erase(key);
    }


//    struct Iterator {
//    };

    typedef typename std::unordered_map<Key, Value, Hash>::iterator Iterator;

    std::pair<Iterator, bool> find(Key key) {
        auto f = mapper.find(key);
        if(f==mapper.end()) return {mapper.end(),nullptr};
        return {f,true};
    }

    Value getItem(Iterator iter) {
        return *iter;
    }

    void setItem(Iterator iter, Value value) {
        *iter = value;
    }

    bool empty() const {
        return mapper.empty();
    }

    void clear() {
        mapper.clear();
    }
private:
    std::unordered_map<Key, Value, Hash> mapper;
};

//template<typename T>
struct ExistUsers {
    //或许可以加缓存，可以做个缓存实验看看。
    bool addUser(Username username, const User &user) {
        //TODO

        bool has_success = mapper.insert({username, user}).second;
        if (has_success) {
            is_virgin = 0;
            have_been_changed = 1;
        }
        return has_success;
    }

    std::pair<User, bool> getUser(Username username) {
        if (!have_been_changed && last_query_username == username) {
            throw EfficiencyError_GetUserTwice();
            //若不想费心，可改为 return {last_query_user,1};
        }
        auto ptr = mapper.find(username);
        if (ptr == mapper.end()) return {User(), 0};
        have_been_changed = 0;
        last_query_user = ptr->second;
        return {ptr->second, 1};
    }

    bool isUserExist(Username username) { return getUser(username).second; }

    pair<User, int> changeInfo(Username username, Password password, Name name, MailAddr mailAddr,
                               Privilege privilege, Privilege required_privilege_bigger_than_privilege) {
        auto ptr = mapper.find(username);
        if (ptr == mapper.end()) return {User(), 1};
        if (ptr->second.privilege >= required_privilege_bigger_than_privilege) return {User(), 2};
        User previous_user = ptr->second;
        const User &changed_user = User(privilege == -1 ? previous_user.privilege : privilege,
                                        name == "" ? previous_user.name : name,
                                        mailAddr == "" ? previous_user.mailAddr : mailAddr,
                                        password == "" ? previous_user.password : password);
        ptr->second = changed_user;//FIXME 到时候这个指针应该改成一个函数什么的，因为做不到引用传参嘛
        have_been_changed = 1;
        return {changed_user, 0};
        //将username 对应的 user 改变其Password password, Name name, MailAddr mailAddr,
        //                    Privilege privilege， 并要求required_privilege_bigger_than_this > 用户的原privelege
        //如果有为“”的值默认不变,如果privilege == -1默认不变
        //if success, return true 0
        //else if no user exist return 1
        //else if no privilege return 2
        //TODO
    }

    bool empty() {
        return mapper.empty();
    }

    void clear() {
        have_been_changed = 0, is_virgin = 1;
        mapper.clear();
    }

    bool isFirstUser() {
        return is_virgin;
    }

private:
    std::unordered_map<Username, User, std::hash<std::string>> mapper;
    bool have_been_changed = 1;//指上一次操作是否为写操作
    bool is_virgin = 1;//指是否它还没有进行过任何addUser操作
    Username last_query_username;//上一次读操作得到的User
    User last_query_user;//上一次读操作得到的User
    class EfficiencyError_GetUserTwice {
    };//存在效率降低的连续两次读取同一username的逻辑失误，则抛出。
} existUsers;


template<class Key, class Value, class Hash>
//Hash是一个模板类名，它实例化后的一个对象例为auto h = Hash<string>(), 这个对象重载了括号，比如可以h(1),然后返回一个size_t
class InnerUniqueUnorderMap {
public:

    bool insert(const std::pair<Key, Value> &pair) {
        return mapper.insert(pair).second;
    }

    bool erase(const Key &key) {
        return mapper.erase(key);
    }

    bool empty() const {
        return mapper.empty();
    }

    void clear() {
        mapper.clear();
    }

    Value *find(Key key) {
        auto f = mapper.find(key);
        if(f==mapper.end()) return nullptr;
        return &(f->second);
    }

private:
    std::unordered_map<Key, Value, Hash> mapper;
};
InnerUniqueUnorderMap<Username, Privilege, std::hash<std::string>> loooooooginUsers;

/*//stub
struct LoginUsers {
    //应该再有一个用户的bought tickets的信息
    //或许可以加缓存
    bool loginUser(Username username, Privilege privilege) {
        bool has_success = mapper.insert({username, privilege}).second;
        if (has_success)have_been_changed = 1;
        return has_success;
    }

    bool logoutUser(Username username) {
        bool has_success = mapper.erase(username);
        if (has_success)have_been_changed = 1;
        return has_success;
    }

    std::pair<Privilege, bool> getUser(Username username) {
        if (!have_been_changed && last_query_username == username) {
            throw EfficiencyError_GetUserTwice();
            //若不想费心，可改为 return {last_query_privilege,1};
        }
        auto ptr = mapper.find(username);
        if (ptr == mapper.end()) return {-1, 0};
        have_been_changed = 0;
        last_query_privilege = ptr->second;
        return {ptr->second, 1};
    }

    Privilege getPrivilege(Username username) {
        auto u = getUser(username);
        return (u.second) ? u.first : -1;
    }

    bool isUserExist(Username username) { return getUser(username).second; }

    int changePrivilege(Username username, Privilege privilege, Privilege required_privilege_bigger_than_privilege) {
        //if success, return true 0
        //else if no user exist return 1
        //else if no privilege return 2
        auto ptr = mapper.find(username);
        if (ptr == mapper.end()) return 1;
        if (ptr->second >= required_privilege_bigger_than_privilege) return 2;
        ptr->second = privilege;
        have_been_changed = 1;
        return 0;
    }

    bool empty() {
        return mapper.empty();
    }

    void clear() {
        have_been_changed = 0;
        mapper.clear();
    }

private:
    std::unordered_map<Username, Privilege, std::hash<std::string>> mapper;
    bool have_been_changed = 1;
    Username last_query_username;//上一次读操作得到的User
    Privilege last_query_privilege;

    class EfficiencyError_GetUserTwice {
    };
} loginUsers;*/

InnerUniqueUnorderMap<Username, Privilege, std::hash<std::string>> loginUsers;

struct Train {
    static constexpr int STATIONMAX = 101;
    static constexpr int DAYMAX = 65;
    StationNum stationNum;
    StationName stations[STATIONMAX];//这里可以用表示替代，注意！
    SeatNum seatNum;
    Price prices[STATIONMAX];
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
        for (int i = 0; i < stationNum - 1; ++i) prices[i] = _prices[i];
        arrivingTimes[0] = leavingTimes[stationNum - 1] = -1;
        arrivingTimes[1] = _arrivingTimes[0], leavingTimes[0] = 0;
        for (int i = 1; i <= stationNum - 2; ++i) {
            leavingTimes[i] = arrivingTimes[i] + _leavingTimes[i - 1];
            arrivingTimes[i + 1] = leavingTimes[i] + _arrivingTimes[i];
        }
        startSaleDate = _saleDate[0], endSaleDate = _saleDate[1];

    }
};

//typedef  TrainPtr;
//struct ExistTrains {
//    std::pair<Train, bool> findTrain(TrainID trainID){
//
//    }
//    void addTrain(TrainID trainID, const Train& train){
//
//    }
//    bool deleteTrain(TrainID trainID){}
//    TrainPtr getTrainPtr(TrainID trainID){}
//    void queryTrain(TrainID trainID){}
//} existTrains;

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
