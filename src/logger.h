//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_LOGGER_H


#include <iostream>
#include "filemanip.h"
#include "BasicHeader.h"
/*
 * AnsCheck为文件对标机开关，需要把data文件夹放入目录使用
 * FileI为读入文件路径，如果想控制台输入只需要把这行注掉
 * fans为答案文件路径，与AnsCheck联合使用（当AnsCheck被关闭时自动失效）
 */
#define AnsCheck
#define FileI { std::freopen("../data/basic_1/1.in", "r", stdin);};
std::fstream fans("../data/basic_1/1.out");
//#define TimeTracing





auto RED = "\033[0;31;1m";
auto GREEN = "\033[0;32;1m";
auto YELLOW = "\033[0;33;1m";
auto BLUE = "\033[0;34;1m";
auto END = "\033[0m";
auto CUT = "---------------------------------------------\n";



std::ofstream main_log("log.dat", std::ios::app);

#define putInfoTo main_log


#define check1(x) putInfoTo << #x << "=" << x << "    ";if(std::string(x)==""){Error("INVALID PARAMETER ");}
#define check2(x, y) check1(x)check1(y)
#define check3(x, y, z) check2(x,y)check1(z)
#define check4(x, y, z, a) check3(x,y,z)check1(a)
#define check5(x, y, z, a, b) check4(x,y,z,a)check1(b)
#define check6(x, y, z, a, b, c) check5(x,y,z,a,b)check1(c)
#define check7(x, y, z, a, b, c, d) check6(x,y,z,a,b,c)check1(d)
#define check8(x, y, z, a, b, c, d, e) check7(x,y,z,a,b,c,d)check1(e)
#define check9(x, y, z, a, b, c, d, e, f) check8(x,y,z,a,b,c,d,e)check1(f)
#define check10(x, y, z, a, b, c, d, e, f, g) check9(x,y,z,a,b,c,d,e,f)check1(g)
#define check11(x, y, z, a, b, c, d, e, f, g, h) check10(x,y,z,a,b,c,d,e,f,g)check1(h)
#define cks(num, ...) do{putInfoTo <<__FUNCTION__ << ": ";check##num(__VA_ARGS__) putInfoTo << std::endl;}while(0)
#define ck(x) do{putInfoTo <<__FUNCTION__ << ": ";check1(x) putInfoTo << std::endl;} while(0)

#define checkint1(x) putInfoTo << #x << "=" << x << "    ";if(x==-1){Error("INVALID PARAMETER");}
#define checkint2(x, y) checkint1(x)checkint1(y)
#define checkint3(x, y, z) checkint2(x,y)checkint1(z)
#define checkint4(x, y, z, a) checkint3(x,y,z)checkint1(a)
#define ckints(num, ...) do{checkint##num(__VA_ARGS__) putInfoTo << std::endl;}while(0)
#define ckint(x) do{checkint1(x) putInfoTo << std::endl;} while(0)



//-----------------------------------------------TIME TRACING
#ifdef TimeTracing
#include <ctime>
#include <map>
map<const char*, int> time_recorder;
map<const char*, int> function_called_num;
int AAABBBCCC;
int* functionToBeTimed= &AAABBBCCC;
clock_t tClockTimer = clock();
#define ResetClock *functionToBeTimed += clock() - tClockTimer;tClockTimer = clock();functionToBeTimed = &time_recorder[__FUNCTION__];++function_called_num[__FUNCTION__]
#define DisplayClock for(auto [functionName, timeSpend]:time_recorder) main_log << BLUE << string("                      " ).replace(0,string(functionName).length(),functionName) << "\t\ttime: " << timeSpend << "   \t\thitnumber: " << function_called_num[functionName] << "\t\taverage time: " << timeSpend/function_called_num[functionName] << std::endl
#define StartStopWatch auto functimer = [&](){ResetClock
#define EndStopWatch };functimer()
#else
#define ResetClock
#define DisplayClock
#endif

//-----------------------------------------------LOG
inline void log() {
    DisplayClock;
    main_log << std::flush;
    std::ifstream fin("log.dat");
    std::string s;
    while (!fin.eof()) {
        getline(fin, s);
        std::cout << s << std::endl;
    }
    fin.close();
    fclear("log.dat");
}

inline void cleanlog() {
    fclear("log.dat");
}

//-----------------------------------------------OUTPUT
inline void Info(std::string x)    {main_log << BLUE << x << END << std::endl;}

class ErrorOccur {
};
inline void Error(const char *x) {
    main_log << RED << "error: " << x << END << std::endl << CUT;
    throw ErrorOccur();
}

template<class T>
void Return(T thing){
    std::cout << thing << std::endl;
#ifdef AnsCheck
    std::string ansstr, mystr;
    std::getline(fans, ansstr);
    std::stringstream ss;
    ss << thing;
    mystr = ss.str();
    if(mystr!=ansstr){
        main_log << RED  << "FAIL " << mystr << END << std::endl;
        main_log << RED  << "ANS= " << ansstr << END << std::endl << CUT;
        std::cout << RED << "DIFFERENT ANSWER\n" << "ANSWER IS: " << ansstr << "\nIWRONG IS: " << mystr << std::endl;
        log();
        std::exit(0);
    }
#endif
    main_log << GREEN  << "success " << thing << END << std::endl << CUT;
}

#define CODE_LOGGER_H

#endif //CODE_LOGGER_H
