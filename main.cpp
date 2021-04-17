#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <regex>
#include <fstream>
#include <sstream>
#include "logger.h"

using namespace std;

void initialize();

void function_chooser();

class ErrorOccur {
};

ofstream main_log("log.dat", ios::app);
ofstream finance_log("financelog.dat", ios::app);
ofstream operation_log("operation.dat", ios::app | ios::binary);

namespace user {

    void login(Username, Password);

    void logout(Username);

    void add_user(Username, Username, Password, Name, MailAddr, Privilege);

    void query_profile(Username, Username);

    void modify_profile(Username, Username, Password, Name, MailAddr, Privilege);

}

namespace train {
    void add_train(TrainID,StationNum,SeatNum,Stations,Prices,StartTime,TravelTimes,StopoverTimes,SaleDates,Type);

    void release_train(TrainID);

    void query_train();

    void delete_train();

    void query_ticket();

    void query_transfer();

    void buy_ticket();

    void query_order();

    void refund_ticket();
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
            cout << "Invalid" << endl;
        }
    }
}

string input;

void initialize() {

}

void function_chooser() {
    smatch parameter;
    static const string
            username = " ([a-z]\\w{1,19})", _c = " -c" + username, _u = " -u" + username,
            passwd = " (\\w{6,30})", _pu = " -p" + passwd,
            name = " ([\\u4e00-\\u9fa5]{2,5})", _nu = " -n" + name,
            mailAddr = " [0-9a-zA-Z@\\.]{1,30}", _mu = " -m" + mailAddr,
            privilege = " (10|0-9)", _g = " -g" + privilege;
    static const string
            trainID = username, _i = " -i" + trainID,
            stationNum = " (100|[1-9][0-9]|[2-9])", _n = " -n" + stationNum, _num = " -n (\\%d+)",
            station = " ([\\u4e00-\\u9fa5]{1,10})",
            seatNum = " (100000|[1-9]\\d{0,4}|0)", _m = " -m" + seatNum,
            price = seatNum, _p = " -p" + price, _pt = " -p (time|cost)",
            startTime = " ((?:[0-1][0-9]|2[0-3]|[0-9]):[0-5][0-9])", _x = " -x"+startTime, _st=" _s"+startTime,
            travelTime = " (10000|[1-9]\\\\d{0,3}|0)", _t = " -t" + travelTime,
            stopoverTime = travelTime, _o = " -t" + stopoverTime,
            saleDate = " (06-(?:0[1-9]|[1-2][0-9]|30)|0[7-8]-(?:0[1-9]|[1-2][0-9]|3[0-1]))", _d = " -d" + saleDate,
            type = " [A-Z]", _y = " -y" + type;
    getline(cin, input);
    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);
    if (cin.eof()) exit(0);
    if (input == "") return;
    Info("try to " + input);
    static auto match = [&parameter](const string &str) -> bool {return regex_search(input, parameter, regex("^" + str));};
    static auto hasparm = [&parameter](const string &str) -> bool {return regex_search(input, parameter, regex(str));};
    static auto pm = [&parameter](const string &str) -> string {if (hasparm(str)) return parameter.str(1); return string();};
    static auto parmint = [&parameter](const string &str) -> int {if (hasparm(str)) return stoi(parameter.str(1));return -1;};
#define trymatch(str,func) if(match(str)){func;return;}
    trymatch("add_user", user::add_user(pm(_c), pm(_u), pm(_pu), pm(_nu), pm(_mu), parmint(_g)))
    trymatch("login", user::login(pm(_u), pm(_pu)))
    trymatch("logout", user::logout(pm(_u)))
    trymatch("query_profile", user::query_profile(pm(_c), pm(_u)))
    trymatch("modify_profile", user::modify_profile(pm(_c), pm(_u), pm(_pu), pm(_nu), pm(_mu), parmint(_g)))
    trymatch("add_train", train::add_train(pm(_i), pm(_n), pm(_m), pm(_ss), pm(_ps), pm(_x), pm(_ts), pm(_os), pm(_ds), pm(_y)))
    trymatch("release_train", train::release_train(pm(_i)))
    trymatch("query_train", train::query_train(pm(_i), pm(_d)))
    trymatch("query_transfer", user::logout(pm(_u)))//FIXME
    trymatch("buy_ticket", train::buy_ticket(parameter.str(1), stoi(parameter.str(2))))
    trymatch("refund_ticket", train::refund_ticket())
    trymatch("log", sys::log())
    trymatch("exit", (cout << "bye" << endl, exit(0)))FIXME
    trymatch("clean", sys::clean())
#undef trymatch
    Error("SYNTAX ERROR");
    throw ErrorOccur();
}


void sys::log() {
    FLUSHLOG;
    ifstream fin("log.dat");
    string s;
    while (!fin.eof()) {
        getline(fin, s);
        cout << s << endl;
    }
    fin.close();
    Success;
}

void sys::clean(){

}
void sys::exit() {
    cout << "bye" << endl;
    std::exit(0);
}


