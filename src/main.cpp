

#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <regex>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "vector.hpp"


void initialize();

void function_chooser();


namespace user {

    void login(Username, Password);

    void logout(Username);

    void add_user(Username, Username, Password, Name, MailAddr, Privilege);

    void query_profile(Username, Username);

    void modify_profile(Username, Username, Password, Name, MailAddr, Privilege);

}

namespace train {

    void
    add_train(TrainID, StationNum, SeatNum, StationNames, Prices, StartTime, TravelTimes, StopoverTimes, SaleDates,
              Type);

    void release_train(TrainID);

    void query_train(TrainID, MonthDate);

    void delete_train(TrainID);

    void query_ticket(StationName, StationName, MonthDate, TwoChoice);

    void query_transfer(StationName, StationName, MonthDate, TwoChoice);

    void buy_ticket(Username, TrainID, MonthDate, TicketNum, StationName, StationName, TwoChoice);

    void query_order(Username);

    void refund_ticket(Username, OrderNumth);
}

namespace sys {

    void noReturnClean();

    void clean();

    void exit();
}


int main() {
#ifdef FileI
    FileI
#endif
    initialize();
    while (true) {
        try {
            function_chooser();
        }
        catch (ErrorOccur) {
            Return(-1);
        }
    }
}

std::string input;

void initialize() {
    sys::noReturnClean();//FIXME used to debug
}

void function_chooser() {//FIXME 时间性能异常，首先要把所有regex东西都提出来改成static使她快20倍，而即使这样也特别的慢。
    ResetClock;
    std::smatch parameter;
    static auto pluralStrMaker = [](const std::string &str) -> const std::string {
        const std::string strNoSpace = str.substr(1);
        return " (" + strNoSpace + "(?:\\|" + strNoSpace + ")*)";
    };
    static const std::string
            chinese = "[^\\s\\|]{3}",
//            chinese = "\\w/*[\u4e00-\u9fa5]*/",
    username = " ([a-zA-z]\\w{0,19})",
            passwd = " (\\S{1,30})",
            name = " (" + chinese + "{2,5})",
            mailAddr = " ([0-9a-zA-Z\\@\\.]{1,30})",
            privilege = " (10|[0-9])";
    static const std::regex
            _c(" -c" + username),
            _u(" -u" + username),
            _pu(" -p" + passwd),
            _nu(" -n" + name),
            _mu(" -m" + mailAddr),
            _g(" -g" + privilege),
            rule_add_user("^add_user"),
            rule_login("^login"),
            rule_logout("^logout"),
            rule_query_profile("^query_profile"),
            rule_modify_profile("^modify_profile");
    static const std::string
            trainID = username,
            stationNum = " (100|[1-9][0-9]|[2-9])",
            station = " (" + chinese + "{1,10})",
            seatNum = " (100000|[1-9]\\d{0,4}|0)",
            price = seatNum,
            startTime = " ((?:[0-1][0-9]|2[0-3]|[0-9]):[0-5][0-9])",
            travelTime = " (10000|[1-9]\\d{0,3}|0)",
            stopoverTime = travelTime,
            saleDate = " (06-(?:0[1-9]|[1-2][0-9]|30)|0[7-8]-(?:0[1-9]|[1-2][0-9]|3[0-1]))",
            type = " ([A-Z])";
    static const std::regex
            _i(" -i" + trainID),
            _n(" -n" + stationNum), _num(" -n (\\%d+)"),
            _startPlace(" -s" + station),
            _fromPlace(" -f" + station), _toPlace(" -t" + station), _ss(" -s" + pluralStrMaker(station)),
            _m(" -m" + seatNum),
            _p(" -p" + price), _pt(" -p (time|cost)"), _qt(" -q (false|true)"),
            _ps(" -p" + pluralStrMaker(price)),
            _x(" -x" + startTime), _s(" -s" + startTime),
            _t(" -t" + travelTime),
            _ts(" -t" + pluralStrMaker(travelTime)),
            _o(" -o" + stopoverTime), _os(" -o" + pluralStrMaker(stopoverTime)),
            _d(" -d" + saleDate), _ds(" -d" + pluralStrMaker(saleDate)),
            _y(" -y" + type),
            rule_add_train("^add_train"),
            rule_release_train("^release_train"),
            rule_query_train("^query_train"),
            rule_delete_train("^delete_train"),
            rule_query_ticket("^query_ticket"),
            rule_query_transfer("^query_transfer"),
            rule_buy_ticket("^buy_ticket"),
            rule_query_order("^query_order"),
            rule_refund_ticket("^refund_ticket"),
            rule_log("^log"),
            rule_clean("^clean"),
            rule_exit("^exit");

    getline(cin, input);
    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);
    if (cin.eof()) exit(0);
    if (input == "") return;
    Info(input);
    //FIXME 这里不用每次都解析正则，改成static，或许是有很微小的优化空间的

    static auto match = [&parameter](const std::regex &str) -> bool {//FIXME match这一步依然是耗时的大头

        return std::regex_search(input, parameter, str);
    };
    static auto pm = [&parameter](const std::regex &str) -> std::string {
//        ResetClock;
        if (match(str))
            return parameter.str(1);
        return std::string();
    };
    static auto pmint = [&parameter](const std::regex &str) -> int {
//        ResetClock;
        if (match(str)) return stoi(parameter.str(1));
        return -1;
    };
#define trymatch(str, func) if(match(str)){func;ResetClock;return;}
    trymatch(rule_add_user, user::add_user(pm(_c), pm(_u), pm(_pu),
                                           pm(_nu),
                                           pm(_mu),
                                           pmint(_g)))
    trymatch(rule_login, user::login(pm(_u), pm(_pu)))
    trymatch(rule_logout, user::logout(pm(_u)))
    trymatch(rule_query_profile, user::query_profile(pm(_c), pm(_u)))
    trymatch(rule_modify_profile, user::modify_profile(pm(_c), pm(_u), pm(_pu), pm(_nu), pm(_mu), pmint(_g)))

    trymatch(rule_add_train,
             train::add_train(pm(_i), pmint(_n), pmint(_m), pm(_ss), pm(_ps), pm(_x), pm(_ts), pm(_os), pm(_ds),
                              pm(_y)))
    trymatch(rule_release_train, train::release_train(pm(_i)))
    trymatch(rule_query_train, train::query_train(pm(_i), pm(_d)))
    trymatch(rule_delete_train, train::delete_train(pm(_i)))
    trymatch(rule_query_ticket, train::query_ticket(pm(_startPlace), pm(_toPlace), pm(_d), pm(_pt)))//FIXME
    trymatch(rule_query_transfer, train::query_transfer(pm(_startPlace), pm(_toPlace), pm(_d), pm(_pt)))//FIXME
    trymatch(rule_buy_ticket,
             train::buy_ticket(pm(_u), pm(_i), pm(_d), pmint(_num), pm(_fromPlace), pm(_toPlace), pm(_qt)))
    trymatch(rule_query_order, train::query_order(pm(_u)))//FIXME
    trymatch(rule_refund_ticket, train::refund_ticket(pm(_u), pmint(_num)))
    trymatch(rule_log, log())
    trymatch(rule_clean, sys::clean())
    trymatch(rule_exit, sys::exit())
#undef trymatch
    Error("SYNTAX ERROR");
}

void user::add_user(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                    Privilege privilege) {
    ResetClock;
    cks(4, username, password, name, mailAddr);
    if (!existUsers.empty()) {//这里把它的语义改了下，不过应该无伤大雅，本来要判的是isFirstUser()的
        ck(cur_username);
        ckint(privilege);
        Privilege curPrivilege = *(loginUsers.find(cur_username));
        if (curPrivilege == -1)Error("CURRENT USER DOES NOT LOGIN");
        if (privilege >= curPrivilege)Error("NO PRIVILEGE");
//        if (password.size() < 6)Error("PASSWORD IS TOO SHORT");
    } else {
        privilege = 10;
        ckint(privilege);
    }
    existUsers.insert({username, User(privilege, name, mailAddr, password)});
    Return(0);

}

void user::login(Username username, Password password) {
    ResetClock;
    cks(2, username, password);
    auto CurUserPair = existUsers.find(username);
    if (!CurUserPair.second) Error("USER DOES NOT EXIST");
    const User &foundUser = existUsers.getItem(CurUserPair.first);
    if (foundUser.password != password) Error("WRONG PASSWORD");
    if (!loginUsers.insert({username, foundUser.privilege}))Error("USER HAS ALREADY LOGIN");
    Return(0);

}

void user::logout(Username username) {
    ResetClock;
    ck(username);
    if (!loginUsers.erase(username))Error("CURRENT USER DOES NOT LOGIN");
    Return(0);

}


void user::query_profile(Username cur_username, Username username) {
    ResetClock;
    cks(2, cur_username, username);
    auto curUserPtr = loginUsers.find(cur_username);
    if (!curUserPtr) Error("CURRENT USER DOES NOT LOGIN");
    auto UserPair = existUsers.find(username);
    if (!UserPair.second) Error("FINDING USER DOES NOT EXIST");
    const User &foundUser = existUsers.getItem(UserPair.first);
    Privilege &curPrivilege = *curUserPtr;
    if (!(curPrivilege > foundUser.privilege || cur_username == username)) Error("NO PRIVILEGE");
    Return(username + ' ' + foundUser.name + ' ' + foundUser.mailAddr + ' ' +
           std::to_string(foundUser.privilege));

}

void user::modify_profile(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                          Privilege privilege) {
    ResetClock;
    cks(2, cur_username, username);
    auto curUserPtr = loginUsers.find(cur_username);
    if (!curUserPtr) Error("CURRENT USER DOES NOT LOGIN");
    Privilege curPrivilege = (cur_username == username) ? -2 : *curUserPtr;//-2表示解禁，小于任何权限

    auto ptr = existUsers.find(username);
    if (!ptr.second) Error("FINDING USER DOES NOT EXIST");
    User previous_user = existUsers.getItem(ptr.first);
    if (previous_user.privilege >= curPrivilege) Error("NO PRIVILEGE");
    const User &changedUser = User(privilege == -1 ? previous_user.privilege : privilege,
                                   name == "" ? previous_user.name : name,
                                   mailAddr == "" ? previous_user.mailAddr : mailAddr,
                                   password == "" ? previous_user.password : password);
    existUsers.setItem(ptr.first, changedUser);
    if (privilege != -1) {
        auto ptr = loginUsers.find(username);
        if (ptr)
            *ptr = privilege;//better 可改完上面后更短
    }
    Return(username + ' ' + changedUser.name + ' ' + changedUser.mailAddr + ' ' +
           std::to_string(changedUser.privilege));//显然应该以她现在的名字返回啊

}

template<class T>
sjtu::vector<T> words_spliter(const std::string &_keyword) {
    sjtu::vector<T> ret;
    std::stringstream ss(_keyword);
    std::string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(oneword);
    }
    return ret;
}

sjtu::vector<int> ints_spliter(const std::string &_keyword) {
    sjtu::vector<int> ret;
    std::stringstream ss(_keyword);
    std::string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(stoi(oneword));
    }
    return ret;
}

void train::add_train(TrainID trainID, StationNum stationNum, SeatNum seatNum, StationNames stations, Prices prices,
                      StartTime startTime, TravelTimes travelTimes, StopoverTimes stopoverTimes, SaleDates saleDates,
                      Type type) {
    ResetClock;
    cks(8, trainID, stations, prices,
        startTime, travelTimes, stopoverTimes, saleDates,
        type);
    ckints(2, stationNum, seatNum);
    sjtu::vector<StationName> station_s = words_spliter<StationName>(stations);
    sjtu::vector<ArrivingTime> travelTime_s = ints_spliter(travelTimes);
    sjtu::vector<LeavingTime> stopoverTime_s = ints_spliter(stopoverTimes);
    sjtu::vector<Price> price_s = ints_spliter(prices);
    sjtu::vector<SaleDate> saleDate_s = words_spliter<SaleDate>(saleDates);
    if (!(station_s.size() == stationNum && price_s.size() == stationNum - 1 && travelTime_s.size() == stationNum - 1
          && stopoverTime_s.size() == std::max(1, stationNum - 2))) {
        Error("NUM OF '|' DOES NOT MATCH");
    }

    if(!existTrains.insert({trainID, Train(stationNum, station_s, seatNum, price_s, startTime, travelTime_s, stopoverTime_s,
                                       saleDate_s, type)}))Error("TRAIN ALREADY EXIST!");
    Return(0);
}

TrainPtr getTrainPtr(TrainID trainID) {
    auto curTrainPair = existTrains.find(trainID);
    if (!curTrainPair.second) Error("FINDING TRAIN DOES NOT EXIST");
    return curTrainPair.first;
}

void AssureLogin(Username username) {
    if (!loginUsers.find(username))Error("USER DOES NOT LOGIN");
}

void train::release_train(TrainID trainID) {
       ResetClock;
       ck(trainID);
       TrainPtr trainPtr = getTrainPtr(trainID);
       Train train = existTrains.getItem(trainPtr);
       if(train.is_released == 1)Error("TRAIN HAS ALREADY BE RELEASED");
       train.is_released = 1;
       int duration = train.endSaleDate - train.startSaleDate + 1;
       for(int i = 0; i < train.stationNum - 1; ++i)
           for (int j = 0; j < duration; ++j)
               train.ticketNums[i][j] = train.seatNum;
       existTrains.setItem(trainPtr, train);

//    train.stations...
//    stations.setbit(...);//setaccording to stations
    //TODO
}

void train::query_train(TrainID trainID, MonthDate startingMonthDate) {
    ResetClock;
    cks(2, trainID, startingMonthDate);
    TrainPtr trainPtr = getTrainPtr(trainID);
    Train train = existTrains.getItem(trainPtr);
    if(startingMonthDate < train.startSaleDate || startingMonthDate < train.endSaleDate) Error("QUERY DATE NOT IN SALE DATE");
//FIXME 它好像没说查一个未发车的车是0票还是满票，这里按0票处理
    std::string ans;
    Return(trainID+" " +train.type);
    //TODO 检验日期是否在内
    for (int i = 0; i < train.stationNum; ++i) {
        Return(train.stations[i]+" "+std::string(FullDate(startingMonthDate, train.startTime) += train
                .arrivingTimes[i]) + " -> " + std::string(FullDate(startingMonthDate, train.startTime) += train
                .leavingTimes[i]) + " " + ((i!=train.stationNum) ? to_string(train.priceSumSinceStart[i]) : "x"));
    }
}

void train::delete_train(TrainID trainID) {
    ResetClock;
    ck(trainID);
    if(!existTrains.erase(trainID))Error("DELETE TRAIN DOES NOT EXIST");
    Return(0);
}

void train::query_ticket(StationName fromStation, StationName toStation, MonthDate monthDateWhenStartFromfromStation,
                         TwoChoice sortFromLowerToHigherBy) {
    ResetClock;
    cks(3, fromStation, toStation, monthDateWhenStartFromfromStation);

//    findTrainsArrivingCertainStation(fromStation, monthDateWhenStartFromfromStation);
//    findTrainsArrivingCertainStation(toStation, monthDateWhenStartFromfromStation);

}

void train::query_transfer(StationName fromStation, StationName toStation, MonthDate monthDateWhenStartFromfromStation,
                           TwoChoice sortFromLowerToHigherBy) {
    ResetClock;
    cks(3, fromStation, toStation, monthDateWhenStartFromfromStation);

}

void train::buy_ticket(Username username, TrainID trainID, MonthDate monthDate, TicketNum buyTicketNum,
                       StationName fromStation,
                       StationName toStation, TwoChoice wannaWaitToBuyIfNoEnoughTicket) {
    ResetClock;
    cks(5, username, trainID, monthDate, fromStation, toStation);
    ckint(buyTicketNum);
    AssureLogin(username);
    TrainPtr trainPtr = getTrainPtr(trainID);
//    Todo

    Return(0);
}

void train::query_order(Username username) {
    ResetClock;
    ck(username);
    AssureLogin(username);


}

void train::refund_ticket(Username username, OrderNumth orderNumth) {
    ResetClock;
    ck(username);
    ckint(orderNumth);
    AssureLogin(username);

}


void sys::noReturnClean() {
    cleanlog();
    existUsers.clear();
    loginUsers.clear();
}

void sys::clean() {
    sys::noReturnClean();
    Return(0);
}

void sys::exit() {
    Return("bye");
    log();//FIXME to debug
    std::exit(0);
}

//FIXME1.重构log文件结构
//FIXME2.wrapp unwrap函数
//FIXME3.更提出map类