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
#include <stdexcept>

using namespace std;

namespace pl2cpp {

//#include "libexport.h"

// include SWI-prolog.h
#include <SWI-Prolog.h>

// class overview
class Atom;
class Engine;
class Predicate;
class Query;
class Term;


// Global functions
void pl_init(int argc, char** argv);
void my_exception(qid_t q);
void my_exception(const char* text);

struct Engine {

    enum {
        running, halted
    } status;
    Query* actQuery;
    void init(int argc, char** argv);
    void init(int argc, char** argv, char* sourceFile);
    void init(char* programName, char* sourceFile);
    void halt();
//    bool running() {
//        return status == running ? true : false;
//    };
    Engine();
    ~Engine(); // set state
};

extern Engine pl_engine;

/**
 * Class Atom. Represents Prolog atom.
 */
class Atom {
protected:
    atom_t handle;
public:

    Atom() {
        handle = (atom_t) - 1;
    }

    Atom(atom_t h) {
        handle = h;
    }

    Atom(Atom &h) {
        handle = h.handle;
    }

    Atom(const char * text) {
        handle = PL_new_atom(text);
    }

    Atom(string text) {
        handle = PL_new_atom(text.c_str());
    }
    // TODO: jeste pro term?

    // operators
    // 

    operator const char *(void) const {
        return PL_atom_chars(handle);
    }
    // Equality

    bool operator ==(const Atom &a) const {
        return handle == a.handle;
    }

    bool operator !=(const Atom &a) const {
        return handle != a.handle;
    }

    // friends
    friend class Term;
};

/**
 * Class Term.
 */
class Term {
protected:
    term_t handle;
    int size;
public:
    Term();
    Term(const char *);
    Term(const string &);
    Term(const long &);
    Term(const int &);
    Term(const double &);
    //    Term(void * const  &);
    Term(const Atom &);
    //    Term(const Functor &);
    Term(term_t t);

    // Term from args

    Term(Term &t) {
        handle = t.handle;
    }

    Term(Term &t1, Term &t2) {
        handle = PL_new_term_refs(2);

        PL_put_term(handle, t1.handle);
        PL_put_term(handle + 1, t2.handle);

        size = 2;
    }

    Term(Term* arr, int length) {
        handle = PL_new_term_refs(length);

        for (int i = 0; i < length; i++) {
            PL_put_term(handle + i, arr[i].handle);
        }
        size = length;
    }

    // unification
    template <typename T>
    bool unify(const T&) const;

    // ** const na konci rika, ze metoda nemuze menit clenske promenne
    /**
     * Convert Term to C string representation.
     * @return char* C string representation of Term
     */
    const char* to_str() const;
    /**
     * Convert Term to C++ string representation.
     * @return 
     */
    string to_string() const;

    // operators
    // conversion
    inline operator const char* () const;
    operator string() const;
    operator long () const;
    operator int () const;
    operator double () const;
    operator Atom() const;

    // assign

    Term& operator =(const Term& t) {
        handle = t.handle;
    }

    Term operator [] (int index) const {
        if (index < 0 || index >= size) {
            my_exception("Term operator []: index out of range.");
            Term rterm(0);
            return rterm;
        }

        term_t ret = PL_new_term_ref();
        //        PL_get_arg(index+1, handle, ret);
        Term rterm(handle + index);
        return rterm;
    }

    // equality

    bool operator ==(const Term& t) const {
        return PL_compare(handle, t.handle) == 0;
    }

    bool operator !=(const Term& t) const {
        return PL_compare(handle, t.handle) != 0;
    }

    bool operator>(const Term& t) const {
        return PL_compare(handle, t.handle) > 0;
    }

    bool operator >=(const Term& t) const {
        return PL_compare(handle, t.handle) >= 0;
    }

    bool operator<(const Term& t) const {
        return PL_compare(handle, t.handle) < 0;
    }

    bool operator <=(const Term& t) const {
        return PL_compare(handle, t.handle) <= 0;
    }

    // friends
    friend class Query;
};

//*************************** Class Term **********************************

inline Term::Term() {
    handle = PL_new_term_ref();
    size = 1;
}

inline Term::Term(const char* str) {
    handle = PL_new_term_ref();
    if (!PL_chars_to_term(str, handle))
        cerr << "Term construct error: parse from C string." << endl;
    size = 1;
}

inline Term::Term(const string& str) {
    handle = PL_new_term_ref();
    if (!PL_chars_to_term(str.c_str(), handle))
        cerr << "Term construct error: parse from C++ string." << endl;
    size = 1;
}

inline Term::Term(const int& val) {
    handle = PL_new_term_ref();
    if (!PL_put_integer(handle, val))
        cerr << "Term construct error: from int." << endl;
    size = 1;
}

inline Term::Term(const long& val) {
    handle = PL_new_term_ref();
    if (!PL_put_integer(handle, val))
        cerr << "Term construct error: from long." << endl;
    size = 1;
}

inline Term::Term(const double& val) {
    handle = PL_new_term_ref();
    if (!PL_put_float(handle, val))
        cerr << "Term construct error: from float." << endl;
    size = 1;
}

inline Term::Term(const Atom& a) {
    handle = PL_new_term_ref();
    if (!PL_put_atom(handle, a.handle))
        cerr << "Term construct error: from Atom." << endl;
    size = 1;
}

inline Term::Term(term_t t) {
    handle = t;
    size = 1;
}


// unification

template <typename T>
inline bool Term::unify(const T& v) const {
    Term tmp(v);
    return unify(tmp);
}

template <>
inline bool Term::unify(const Term& t) const {
    return PL_unify(handle, t.handle);
}


// operators 

inline Term::operator const char* () const {
    //    throw "ble";
    return to_str();
}

inline Term::operator string() const {
    return to_string();
}

inline Term::operator int() const {
    int val;
    if (!PL_get_integer(handle, &val))
        cerr << "Term conversion error: Term to int." << endl;
    return val;
}

inline Term::operator long() const {
    long val;
    if (!PL_get_long(handle, &val))
        cerr << "Term conversion error: Term to long." << endl;
    return val;
}

inline Term::operator double() const {
    double val;
    if (!PL_get_float(handle, &val))
        cerr << "Term conversion error: Term to double." << endl;
    return val;
}

inline Term::operator Atom() const {
    Atom a;
    if (!PL_get_atom(handle, &a.handle))
        cerr << "Term conversion error: Term to Atom." << endl;
    return a;
}

// strings representations

inline const char* Term::to_str() const {
    unsigned flags;
    flags = CVT_VARIABLE | CVT_WRITE | BUF_RING;
    //    if (!(flags & (BUF_DISCARDABLE|BUF_MALLOC|BUF_RING)))
    //        flags |= BUF_RING;
    string str("");
    char* tmp;
    for (int i = 0; i < size; i++) {
        if (!PL_get_chars(handle + i, &tmp, flags))
            cerr << "Term conversion error: Term to C string." << endl;
        str.append(tmp);
    }
    return str.c_str();
}

inline string Term::to_string() const {
    unsigned flags;
    flags = CVT_VARIABLE | CVT_WRITE | BUF_DISCARDABLE;
    //    if (!(flags & (BUF_DISCARDABLE|BUF_MALLOC|BUF_RING)))
    //        flags |= BUF_RING;
    char* str;
    if (!PL_get_chars(handle, &str, flags))
        cerr << "Term conversion error: Term to C string." << endl;
    return string(str);
}

// *************************** class Query **********************************

class Query {
protected:
    qid_t qid; // query id
    predicate_t pred; // predicate id
    int arity; // arity of predicate
    term_t predArgs; // predicate arguments
public:

    Query(const char* predName) {
        if (!pl_engine.actQuery) {
            pl_engine.actQuery = this;
            pred = PL_predicate(predName, 0, NULL);
            term_t a;
            qid = PL_open_query(NULL, PL_Q_CATCH_EXCEPTION, pred, a);
        } else {
            my_exception("Query using error: only one query can be used at the time");
            //            throw "nasraaat";
        }

    }

    Query(const char* predName, int predArity, Term& predArgs) : arity(predArity) {
//        arity = predArity;

        if (!pl_engine.actQuery) {
            pl_engine.actQuery = this;
            pred = PL_predicate(predName, arity, NULL);
            qid = PL_open_query(NULL, PL_Q_CATCH_EXCEPTION, pred, predArgs.handle);
        } else {
            my_exception("Query using error: only one query can be used at the time");
            //            throw "nasraaat";
        }

    }

    ~Query() {
        //        PL_cut_query(qid);
    }

    bool next(bool quiet = true) {

        if (!PL_next_solution(qid)) {
            if (!quiet)
                my_exception("No next solution.");
            return false;
        }
        return true;
    }

    bool call() {
        bool ret = next();
        cut();
        return ret;
    }

    void cut() {
        pl_engine.actQuery = 0;
        PL_cut_query(qid);
    }

    void close() {
        pl_engine.actQuery = 0;
        PL_close_query(qid);
    }
};

// ********************** Class PrologException *****************************

class prologException : public exception {
protected:
    string message;
    Term t;
public:

    prologException(qid_t q) throw () {
        term_t t1 = PL_exception(q);
        if (!t1) {
            // PL_exception returned 0
            message.assign("No exception.");
            return;
        }

        t = t1;
        message.assign(t.to_str());
    }

    prologException(const char* text) {
        message.assign(text);
    }

    ~prologException(void) throw () {
    }

    virtual const char* what() const throw () {
        return message.c_str();
    }
};
// *************************** Global **********************************
// output stream operators

inline ostream& operator <<(ostream& os, const Atom& a) {
    os << (const char*) a;
    return os;
}

inline ostream& operator <<(ostream& os, const Term& t) {
    os << t.to_str();
    return os;
}

bool PRT = true;
bool EXC = true;

inline void my_exception(qid_t q = 0) {
    if (PRT)
        cerr << "Prolog exception raises!" << endl;
    //    if (EXC)
    //        prologException exc = new prologException(q);
    //        throw exc;
}

inline void my_exception(const char* text = "") {

    if (PRT)
        cerr << text << endl;
    //    if (EXC)
    //        throw PrologException(text);
}



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

