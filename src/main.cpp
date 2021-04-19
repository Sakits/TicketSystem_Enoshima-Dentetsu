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
    add_train(TrainID, StationNum, SeatNum, Stations, Prices, StartTime, TravelTimes, StopoverTimes, SaleDates, Type);

    void release_train(TrainID);

    void query_train(TrainID, MonthDate);

    void delete_train(TrainID);

    void query_ticket(Station, Station, MonthDate, TwoChoice);

    void query_transfer(Station, Station, MonthDate, TwoChoice);

    void buy_ticket(Username, TrainID, MonthDate, TicketNum, Station, Station, TwoChoice);

    void query_order(Username);

    void refund_ticket(Username, OrderNumth);
}

namespace sys {
    void log();

    void clean();

    void exit();
}


int main() {
#ifdef FileIO
    FileIO
#endif
    initialize();
    while (true) {
        try {
            function_chooser();
        }
        catch (ErrorOccur) {
            std::cout << "-1" << std::endl;
        }
    }
}

std::string input;

void initialize() {

}

void function_chooser() {
    smatch parameter;
    auto pluralStrMaker = [](const std::string &str) -> const std::string {
        const std::string strNoSpace = str.substr(1);
        return " (" + strNoSpace + "(?:\\|" + strNoSpace + ")*)";
    };
    static const std::string
            chinese = "\\S{3}",
//            chinese = "\\w/*[\u4e00-\u9fa5]*/",
    username = " ([a-zA-z]\\w{1,19})", _c = " -c" + username, _u = " -u" + username,
            passwd = " (\\w{1,30})", _pu = " -p" + passwd,
            name = " (" + chinese + "{2,5})", _nu = " -n" + name,
            mailAddr = " ([0-9a-zA-Z\\@\\.]{1,30})", _mu = " -m" + mailAddr,
            privilege = " (10|0-9)", _g = " -g" + privilege;
    static const std::string
            trainID = username, _i = " -i" + trainID,
            stationNum = " (100|[1-9][0-9]|[2-9])", _n = " -n" + stationNum, _num = " -n (\\%d+)",
            station = " (" + chinese + "{1,10})", _startPlace = " -s" + station,
            _fromPlace = " -f" + station, _toPlace = " -t" + station, _ss = " -s" + pluralStrMaker(station),
            seatNum = " (100000|[1-9]\\d{0,4}|0)", _m = " -m" + seatNum,
            price = seatNum, _p = " -p" + price, _pt = " -p (time|cost)", _qt = " -q (false|true)",
            _ps = " -p" + pluralStrMaker(price),
            startTime = " ((?:[0-1][0-9]|2[0-3]|[0-9]):[0-5][0-9])", _x = " -x" + startTime, _s = " -s" + startTime,
            travelTime = " (10000|[1-9]\\\\d{0,3}|0)", _t = " -t" + travelTime,
            _ts = " -t" + pluralStrMaker(travelTime),
            stopoverTime = travelTime, _o = " -o" + stopoverTime, _os = " -o" + pluralStrMaker(stopoverTime),
            saleDate = " (06-(?:0[1-9]|[1-2][0-9]|30)|0[7-8]-(?:0[1-9]|[1-2][0-9]|3[0-1]))",
            _d = " -d" + saleDate, _ds = " -d" + pluralStrMaker(saleDate),
            type = " [A-Z]", _y = " -y" + type;
    getline(cin, input);
    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);
    if (cin.eof()) exit(0);
    if (input == "") return;
    Info("try to " + input);

    static auto match = [&parameter](const std::string &str) -> bool {
        return regex_search(input, parameter, regex("^" + str));
    };
    static auto hasparm = [&parameter](const std::string &str) -> bool {
        return regex_search(input, parameter, regex(str));
    };
    static auto pm = [&parameter](const std::string &str) -> std::string {
        if (hasparm(str))
            return parameter.str(1);
        return std::string();
    };
    static auto pmint = [&parameter](const std::string &str) -> int {
        if (hasparm(str)) return stoi(parameter.str(1));
        return -1;
    };
#define trymatch(str, func) if(match(str)){func;return;}
    trymatch("add_user", user::add_user(pm(_c), pm(_u), pm(_pu),
                                        pm(_nu),
                                        pm(_mu),
                                        pmint(_g)))
    trymatch("login", user::login(pm(_u), pm(_pu)))
    trymatch("logout", user::logout(pm(_u)))
    trymatch("query_profile", user::query_profile(pm(_c), pm(_u)))
    trymatch("modify_profile", user::modify_profile(pm(_c), pm(_u), pm(_pu), pm(_nu), pm(_mu), pmint(_g)))

    trymatch("add_train",
             train::add_train(pm(_i), pmint(_n), pmint(_m), pm(_ss), pm(_ps), pm(_x), pm(_ts), pm(_os), pm(_ds),
                              pm(_y)))
    trymatch("release_train", train::release_train(pm(_i)))
    trymatch("query_train", train::query_train(pm(_i), pm(_d)))
    trymatch("delete_train", train::delete_train(pm(_i)))
    trymatch("query_ticket", train::query_ticket(pm(_startPlace), pm(_toPlace), pm(_d), pm(_pt)))//FIXME
    trymatch("query_transfer", train::query_transfer(pm(_startPlace), pm(_toPlace), pm(_d), pm(_pt)))//FIXME
    trymatch("buy_ticket",
             train::buy_ticket(pm(_u), pm(_i), pm(_d), pmint(_num), pm(_fromPlace), pm(_toPlace), pm(_qt)))
    trymatch("query_order", train::query_order(pm(_u)))//FIXME
    trymatch("refund_ticket", train::refund_ticket(pm(_u), pmint(_num)))
    trymatch("log", sys::log())
    trymatch("clean", sys::clean())
    trymatch("exit", sys::exit())
#undef trymatch
    Error("SYNTAX ERROR");
}

void user::add_user(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                    Privilege privilege) {
    cks(4, username, password, name, mailAddr);
    if (!existUsers.isFirstUser()) {
        ck(cur_username);
        ckint(privilege);
        Privilege curPrivilege = loginUsers.getPrivilege(cur_username);
        if (curPrivilege == -1)Error("CURRENT USER DOES NOT LOGIN");
        if (privilege >= curPrivilege)Error("NO PRIVILEGE");
        if (password.size() < 6)Error("PASSWORD IS TOO SHORT");
    } else {
        privilege = 10;
        ckint(privilege);
    }
    existUsers.addUser(User(username, privilege, name, mailAddr, password));
    Return(0);
    Success();
}

void user::login(Username username, Password password) {
    cks(2, username, password);
    auto CurUserPair = existUsers.getUser(username);
    if (!CurUserPair.second) Error("USER DOES NOT EXIST");
    User &foundUser = CurUserPair.first;
    if (foundUser.password != password) Error("WRONG PASSWORD");
    if (!loginUsers.loginUser(foundUser))Error("USER HAS ALREADY LOGIN");
    Return(0);
    Success();
}

void user::logout(Username username) {
    ck(username);
    if (loginUsers.logoutUser(username))Error("CURRENT USER DOES NOT LOGIN");
    Return(0);
    Success();
}


void user::query_profile(Username cur_username, Username username) {
    cks(2, cur_username, username);
    auto CurUserPair = loginUsers.getUser(cur_username);
    if (!CurUserPair.second) Error("CURRENT USER DOES NOT LOGIN");
    auto UserPair = existUsers.getUser(username);
    if (!UserPair.second) Error("FINDING USER DOES NOT EXIST");
    User &foundUser = UserPair.first;
    CoreUser &curUser = CurUserPair.first;
    if (!(curUser.privilege > foundUser.privilege || cur_username == username)) Error("NO PRIVILEGE");
    //FIXME 先不做了，逻辑不容易
    Return(string(username) + ' ' + string(foundUser.name) + ' ' + string(foundUser.mailAddr) + ' ' +
           std::to_string(foundUser.privilege));
    Success();
}

void user::modify_profile(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                          Privilege privilege) {
    cks(2, cur_username, username);
    auto curUserPair = loginUsers.getUser(cur_username);
    if (!curUserPair.second) Error("CURRENT USER DOES NOT LOGIN");
    Privilege curPrivilege = (cur_username == username) ? -2 : curUserPair.first.privilege;//-2表示解禁，小于任何权限
    switch (existUsers.changeInfo(username, password, name, mailAddr,
                                  privilege, curPrivilege)) {
        case 0:
            break;
        case 1:
            Error("FINDING USER DOES NOT EXIST");
        case 2:
            Error("NO PRIVILEGE");
    }
    if (privilege != -1) {
        loginUsers.changePrivilege(username, privilege, curPrivilege);
    }
    Return(string(username) + ' ' + string(name) + ' ' + string(mailAddr) + ' ' +
           std::to_string(privilege));
    Success();
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

void train::add_train(TrainID trainID, StationNum stationNum, SeatNum seatNum, Stations stations, Prices prices,
                      StartTime startTime, TravelTimes travelTimes, StopoverTimes stopoverTimes, SaleDates saleDates,
                      Type type) {
    cks(8, trainID, stations, prices,
        startTime, travelTimes, stopoverTimes, saleDates,
        type);
    ckints(2, stationNum, seatNum);
    sjtu::vector<Station> station_s = words_spliter<Station>(stations);
    sjtu::vector<TravelTime> travelTime_s = ints_spliter(travelTimes);
    sjtu::vector<StopoverTime> stopoverTime_s = ints_spliter(stopoverTimes);
    sjtu::vector<Price> price_s = ints_spliter(prices);
    sjtu::vector<SaleDate> saleDate_s = words_spliter<SaleDate>(saleDates);

    Success();
}

void train::release_train(TrainID trainID) {
    ck(trainID);
    Success();
}

void train::query_train(TrainID trainID, MonthDate startingMonthDate) {
    cks(2, trainID, startingMonthDate);

    Success();
}

void train::delete_train(TrainID trainID) {
    ck(trainID);
    Success();

}

void train::query_ticket(Station fromStation, Station toStation, MonthDate monthDateWhenStartFromfromStation,
                         TwoChoice sortFromLowerToHigherBy) {
    cks(3, fromStation, toStation, monthDateWhenStartFromfromStation);

    Success();
}

void train::query_transfer(Station fromStation, Station toStation, MonthDate monthDateWhenStartFromfromStation,
                           TwoChoice sortFromLowerToHigherBy) {
    cks(3, fromStation, toStation, monthDateWhenStartFromfromStation);

    Success();
}

void
train::buy_ticket(Username username, TrainID trainId, MonthDate monthDate, TicketNum buyTicketNum, Station fromStation,
                  Station toStation, TwoChoice wannaWaitToBuyIfNoEnoughTicket) {
    cks(5, username, trainId, monthDate, fromStation, toStation);
    ckint(buyTicketNum);
    AssureLogin(username);

    Success();
}

void train::query_order(Username username) {
    ck(username);
    AssureLogin(username);

    Success();
}

void train::refund_ticket(Username username, OrderNumth orderNumth) {
    ck(username);
    ckint(orderNumth);
    AssureLogin(username);

    Success();
}

void sys::log() {
    FLUSHLOG;
    std::ifstream fin("log.dat");
    std::string s;
    while (!fin.eof()) {
        getline(fin, s);
        std::cout << s << std::endl;
    }
    fin.close();
}

void sys::clean() {
    fclear("log.dat");
    existUsers.clear();
    loginUsers.clear();
    Return(0);
    //unsaletrain.clear train.clear train tickets.clear
}

void sys::exit() {
    std::cout << "bye" << std::endl;
    std::exit(0);
}


