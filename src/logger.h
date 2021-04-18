//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_LOGGER_H


#include <iostream>
#include <ctime>
#include "filemanip.h"
#include "BasicHeader.h"

std::ofstream main_log("log.dat", std::ios::app);

constexpr char* RED    = "\033[0;31;1m";
constexpr char* GREEN  = "\033[0;32;1m";
constexpr char* YELLOW = "\033[0;33;1m";
constexpr char* BLUE   = "\033[0;34;1m";
constexpr char* END    = "\033[0m";

constexpr char* CUT = "---------------------------------------------\n";

inline char* nowtime(){
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

template<class T>
void Return(T thing){
    std::cout << thing << std::endl;
};

#define ACT_INFO   nowtime() << __FUNCTION__

#define Info(x)    main_log << YELLOW  << ACT_INFO  << ": info: " << x << END << std::endl
inline void Success(){    main_log << GREEN  << "success " << END << std::endl << CUT;}

class ErrorOccur {};
inline void Error(const char* x)   {main_log << RED  << "error: " << x << END << std::endl << CUT;throw ErrorOccur();}

#define FLUSHLOG main_log << std::flush


#define putInfoTo main_log

#define check1(x) putInfoTo << #x << "=" << x << "      ";if(std::string(x)==""){Error("INVALID PARAMETER");}
#define check2(x, y) check1(x)check1(y)
#define check3(x, y, z) check2(x,y)check1(z)
#define check4(x, y, z, a) check3(x,y,z)check1(a)
#define check5(x, y, z, a, b) check4(x,y,z,a)check1(b)
#define check6(x, y, z, a, b, c) check5(x,y,z,a,b)check1(c)
#define check7(x, y, z, a, b, c, d) check6(x,y,z,a,b,c)check1(d)
#define check8(x, y, z, a, b, c, d, e) check7(x,y,z,a,b,c,d)check1(e)
#define check9(x, y, z, a, b, c, d, e, f) check8(x,y,z,a,b,c,d,e)check1(f)
#define check10(x, y, z, a, b, c, d, e, f, g) check9(x,y,z,a,b,c,d,e,f)check1(g)
#define check11(x, y, z, a, b, c, d, e, f, g,h) check10(x,y,z,a,b,c,d,e,f,g)check1(h)
#define cks(num,...) do{putInfoTo << "Enter " << __FUNCTION__ << " with parameters: ";check##num(__VA_ARGS__) putInfoTo << std::endl;}while(0)
#define ck(x) do{putInfoTo << "Enter " << __FUNCTION__ << " with parameters: ";check1(x) putInfoTo << std::endl;} while(0)

#define checkint1(x) putInfoTo << #x << "=" << x << "      ";if(x==-1){Error("INVALID PARAMETER");}
#define checkint2(x, y) checkint1(x)checkint1(y)
#define checkint3(x, y, z) checkint2(x,y)checkint1(z)
#define checkint4(x, y, z, a) checkint3(x,y,z)checkint1(a)
#define ckints(num,...) do{checkint##num(__VA_ARGS__) putInfoTo << std::endl;}while(0)
#define ckint(x) do{checkint1(x) putInfoTo << std::endl;} while(0)

#define AssureLogin(u) if(!loginUsers.isUserExist(u))Error("USER DOES NOT LOGIN")

#define CODE_LOGGER_H

#endif //CODE_LOGGER_H
