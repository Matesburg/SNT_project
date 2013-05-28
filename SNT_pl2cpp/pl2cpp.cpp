/* 
 * File:   pl2cpp.cpp
 * Author: Matron
 *
 * Created on 26. květen 2013, 16:19
 */

#include "pl2cpp.h"

// namespace
using namespace pl2cpp;
using namespace std;

// GLOBAL Data
Engine pl2cpp::pl_engine;

// GLOBAL Functions
void pl2cpp::pl_init(int argc, char** argv) {
    pl_engine.init(argc, argv);
}


// Class Engine
void pl2cpp::Engine::init(int argc, char** argv) {
    
    if (!PL_initialise(argc, argv))
        PL_halt(1);
}

void pl2cpp::Engine::halt() {
    if ( status == halted )
        return;
    
    PL_cleanup(0);
    status = halted;
}

pl2cpp::Engine::Engine() {
    status = halted;
}

pl2cpp::Engine::~Engine() {
    halt();
}





void pl2cpp::test(int argc, char** argv) {

    putenv("SWI_HOME_DIR=C:\\swipl");
    char *av[10];
    int ac = 0;

    av[ac++] = argv[0];
    av[ac++] = "-s";
    av[ac++] = "calc.pl";
    //    av[ac++] = "-g";
    //    av[ac++] = "calc('pi/2')";
    av[ac] = NULL;

    if (!PL_initialise(ac, av))
        PL_halt(1);

    cout << "Test lib output";
}

