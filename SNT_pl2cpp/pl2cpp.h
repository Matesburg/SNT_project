/* 
 * File:   pl2cpp.h
 * Author: Matron
 *
 * Created on 26. květen 2013, 16:19
 */

#ifndef PL2CPP_H
#define	PL2CPP_H

// std includes
#include <cstdlib>
#include <iostream>
using namespace std;

namespace pl2cpp {

//#include "libexport.h"

// include SWI-prolog.h
#include <SWI-Prolog.h>

class Atom;
class Engine;
class Predicate;
class Query;
class Term;

/**
 * SWI Prolog engine initialization
 * @param argc
 * @param argv
 */
void pl_init(int argc, char** argv);

struct Engine {
    enum {running, halted} status;
    void init(int argc, char** argv);
    void halt();
    Engine();
    ~Engine(); // set state
};

extern Engine pl_engine;

// ******** DELETE later
/**
 * Test method...
 * @param argc
 * @param argv
 */
void test(int argc, char **argv);
// **************

} // namespace pl2cpp

#endif	/* PL2CPP_H */

