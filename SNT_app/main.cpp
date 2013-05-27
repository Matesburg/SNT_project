/* 
 * File:   main.cpp
 * Author: Matron
 *
 * Created on 26. květen 2013, 15:27
 */

#include <cstdlib>
#include "lib/simlib.h"
#include "../SNT_pl2cpp/pl2cpp.h"
#include <SWI-Prolog.h>
//#include <os>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    putenv("SWI_HOME_DIR=C:\\swipl");
    if( !PL_initialise(argc, argv) )
        PL_halt(1);
    
    test();
//    Init(0, 0.5);
    return 0;
}

