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
    //    pl_init(argc, argv);
    try {
        pl2cpp::test(argc, argv);
    } catch (const char* e) {
        cout << "exceee" << e << endl;
    } catch (exception &e) {
        cout << "PExc " << e.what() << endl;
    }
}

class Machine : public Facility {
public:
    int pos[2];

    Machine(const char* name, int x, int y) : Facility(name) {
        pos[0] = x;
        pos[1] = y;
    }

};




//void Sample() {
//    Print("%g %g\n", Time, 1.5);
//}
//Sampler s(Sample, 100);

class Agent : public Facility {
    int dist;
public:

    Agent(const char* name) : Facility(name) {
    };

    void distance(int x, int y) {
        dist = 0;
        Term tarr[4];
        tarr[0].unify<int>(0);
        tarr[1].unify<int>(0);
        tarr[2].unify<int>(x);
        tarr[3].unify<int>(y);
        Term args(tarr, 4);
                cout << args.to_str() << endl;
        Query q1("searchPath", 4, args);
        if (!q1.next()) {
            my_exception("Calling predicate searchPath error.");
        }
        q1.close();

        Term tx, ty;
        Term tPos(tx, ty);
        Query q2("pos", 2, tPos);
        while (q2.next()) {
            dist++;
            //            cout << "pos:" << tPos.to_str() << endl;
        }
        q2.close();

        Query("clearPos").call();
    }

    double timeToReach(int x, int y) {
        double time = 0.0;
        distance(x, y);
        for (int i = 0; i < dist; i++) {
            time += Exponential(1.0);
        }

        return time;
    }
};

// global objects:
Machine Machine1("Machine1", 2, 1);
Machine Machine2("Machine2", 2, 3);
Agent ag("Agent1");
Histogram Table("Table", 0, 25, 20);

//class MachProcess;

class RefillProcess : public Process {
    double Prichod; // atribute of each customer
//    MachProcess *mProc;
    
    void Behavior() { // --- behavoir specification ---
        Prichod = Time; // incoming time
        
//        cout << target[0] << endl;
        
        Seize(ag); // start of service

//        double ttr = ag.timeToReach(target[0], target[1]);
        double ttr = ag.timeToReach(2, 2);
//        cout << ttr << endl;
        Wait(ttr);
        done = true;
        Wait(ttr);
        Release(ag); // end of service
//        mProc->Activate();
        
        Table(Time - Prichod); // waiting and service time
    }
public:
    bool done;
    int *target;
    RefillProcess(int *pos) : Process() {
        target = pos;
    }
};

class MachProcess : public Process {
    double Prichod; // atribute of each customer
    Machine *mach;
    
    void Behavior() { // --- behavoir specification ---
        Prichod = Time; // incoming time
        
        while(1) {
            Seize(*mach); // start of service

            Wait(5); // time of service

            Release(*mach); // end of service
            
            RefillProcess *rp = new RefillProcess(mach->pos);
            rp->Activate();
            
            WaitUntil(rp->done);
        }
    }
public:

    MachProcess(Machine *m) : Process() {
        mach = m;
    }
};

class Generator : public Event { // model of system's input

    void Behavior() { // --- behavior specification ---
//        (new RefillProcess)->Activate(); // new customer
        (new MachProcess(&Machine1))->Activate(); // new customer
//        Activate(Time + Exponential(1e3 / 150)); //
    }
};

int main(int argc, char** argv) { // experiment description

    // setting up prolog engine
    putenv("SWI_HOME_DIR=C:\\swipl");
    pl_engine.init(argv[0], "calc.pl");

    // simulation
    Print(" model2 - SIMLIB/C++ example\n");
    SetOutput("model2.out");
    Init(0, 1000); // experiment initialization for time 0..1000
    
//    Generator gen;
//    gen.Activate(); // customer generator
    MachProcess mp1(&Machine1);
    mp1.Activate();
    
    Run(); // simulation
    Machine1.Output(); // print of results
    ag.Output(); // print of results
    Table.Output();

    // prolog engine halt
    pl_engine.halt();

    return 0;
}
