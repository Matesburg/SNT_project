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
//void pl2cpp::pl_init(int argc, char** argv) {
//    pl_engine.init(argc, argv);
//}


// Class Engine
void pl2cpp::Engine::init(int argc, char** argv) {
    halt();
    if (!PL_initialise(argc, argv))
        PL_halt(1);
    status = running;
}

void pl2cpp::Engine::init(int argc, char** argv, char* sourceFile) {
    halt();
    
    int ac = 0;
    char* av[4];
    
    av[ac++] = argv[0];
//    av[ac++] = "--quiet";
    av[ac++] = "-s";
    av[ac++] = sourceFile;
    av[ac] = NULL;
    
    if (!PL_initialise(ac, av))
        PL_halt(1);
    
    status = running;
}
void pl2cpp::Engine::init(char* programName, char* sourceFile) {
    halt();
    
    int ac = 0;
    char* av[4];
    
    av[ac++] = programName;
//    av[ac++] = "--quiet";
    av[ac++] = "-s";
    av[ac++] = sourceFile;
    av[ac] = NULL;
    
    if (!PL_initialise(ac, av))
        PL_halt(1);
    
    status = running;
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



// testing
void pl2cpp::test(int argc, char** argv) {

    putenv("SWI_HOME_DIR=C:\\swipl");
    char *av[10];
    int ac = 0;

    av[ac++] = argv[0];
//    av[ac++] = "-s";
//    av[ac++] = "calc.pl";
    //    av[ac++] = "-g";
    //    av[ac++] = "calc('pi/2')";
    av[ac] = NULL;

//    pl_engine.init(ac, av);
    pl_engine.init(argc, argv, "calc.pl");
    
    term_t t = PL_new_term_refs(2);
    char ** out;
    Atom a;
    atom_t at;
    
//    PL_put_atom_chars(t, "pi/2");
//    predicate_t p = PL_predicate("calc", 2, "user");
//        if ( !PL_call_predicate(NULL, PL_Q_CATCH_EXCEPTION, p, t) ) {
////            throw PrologException(2);
//            my_exception("ERROR call predicate");
//        }
//    PL_get_atom(t, &at);
//    const char* outt = PL_atom_chars(at);
////    PL_get_atom_chars(t, out);
//    cout << outt << endl;
//
//    PL_get_atom(t+1, &at);
//    double f;
//    PL_get_float(t+1, &f);
//    cout << f << endl;
    
    
    
    // friend
//    term_t t1 = PL_new_term_ref();
//    p = PL_predicate("friend", 1, NULL);
//    
//    qid_t que = PL_open_query(NULL, PL_Q_CATCH_EXCEPTION, p, t1);
//    if ( !PL_next_solution(que) ) {
////        throw PrologException();
//        mythrow_exception(que);
//    }
//    PL_get_atom(t1, &at);
//    outt = PL_atom_chars(at);
////    PL_get_atom_chars(t, out);
//    cout << outt << endl;
//    
//    
//    PL_next_solution(que);
//    PL_get_atom(t1, &at);
//    outt = PL_atom_chars(at);
////    PL_get_atom_chars(t, out);
//    cout << outt << endl;

//    PL_get_atom(t1+1, &at);
//    f;
//    PL_get_float(t1, &f);
//    cout << f << endl;
    
    
    
    // test unification
    Term tUni;
    Term tUni1("text");
    cout << tUni.to_str() << endl;
    if (tUni.unify<int>(159) )
        cout << tUni.to_str() << endl;
    if (tUni.unify<Term>(tUni1) )
        cout << tUni.to_str() << endl;
    
    
    
    
    
//    Term termAr[2];
//    termAr[0] = Term("pi/2");
//    Term res;
//    termAr[1] = res;
//    Term myt( termAr, 2 );
//    Query qq1("calc", 2, myt);
//    qq1.next();
//    cout << res.to_str() << endl;
////    qq1.cut();
//    qq1.close();
////    
//    Term arr[2];
//    arr[0] = Term("pepa");
//    Term name;
//    arr[1] = name;
//    Term myt1(arr, 2);
//    Query qq2("friend", 2, myt1);
//    qq2.next();
//    qq2.next();
////    Term myt( "testTerm" );
//    cout << name.to_str() << endl;
////
//    Term test;
//    test = myt1[0];
//    cout << "tuuu " << test.to_str() << endl;
    
    Term tarr[4];
    tarr[0].unify<int>(0);
    tarr[1].unify<int>(0);
    tarr[2].unify<int>(3);
    tarr[3].unify<int>(3);
//    tarr[1] = Term(0);
//    tarr[2] = Term(1);
//    tarr[3] = Term(1);
    Term args(tarr, 4);
    cout << args.to_str() << endl;
    Query q1("searchPath", 4, args);
    if ( !q1.next() ) {
        my_exception("Calling predicate searchPath error.");
    }
    q1.close();
    
    Term x, y;
    Term tPos(x, y);
    Query q2("pos", 2, tPos);
    while( q2.next() ) {
        cout << "pos:" << tPos.to_str() << endl;
    }
    q2.close();
    
    cout << "Test lib output" << endl;
    
    pl_engine.halt();
}

