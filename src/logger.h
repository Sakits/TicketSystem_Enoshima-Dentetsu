//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_LOGGER_H


#include <iostream>
#include <ctime>
#include "filemanip.h"
#include "BasicHeader.h"


using namespace std;


constexpr char* RED    = "\033[0;31;1m";
constexpr char* GREEN  = "\033[0;32;1m";
constexpr char* YELLOW = "\033[0;33;1m";
constexpr char* BLUE   = "\033[0;34;1m";
constexpr char* END    = "\033[0m";

constexpr char* CUT = "---------------------------------------------\n";

inline char* nowtime(){
    time_t now = time(0);
    return ctime(&now);
}

#define ACT_INFO   nowtime() << __FUNCTION__

#define Info(x)    main_log << YELLOW  << ACT_INFO  << ": info: " << x << END << '\n'
#define Success    main_log << GREEN  << "success: " << __FUNCTION__ << END << '\n' << CUT
#define Error(x)   main_log << RED  << "error: " << x << END << '\n' << CUT

#define FLUSHLOG main_log << flush


#define CODE_LOGGER_H

#endif //CODE_LOGGER_H
