/* 
 * File:   main.cpp
 * Author: Matron
 *
 * Created on 26. květen 2013, 15:27
 */

//#include <cstdlib>
#include "lib/simlib.h"
#include "../SNT_pl2cpp/pl2cpp.h"

using namespace pl2cpp;
using namespace std;

void test_pl2cpp(int argc, char** argv) {
    putenv("SWI_HOME_DIR=C:\\swipl");
    pl_init(argc, argv);
}

int main(int argc, char** argv) {

    test_pl2cpp(argc, argv);
//    Init(0, 0.5);
    return 0;
}
