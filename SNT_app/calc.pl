calc(Atom, Res) :-
	term_to_atom(Expr, Atom),
        Res is Expr.

friend(martin).
friend(pepa).
friend(tomas).
friend(martin, marta).
friend(pepa, pepina).
friend(Who, Ret) :- friend(Who), Ret is 555 .


:-dynamic pos/2, range/2, barrier/2.

setRange(_,_) :- retract(range(_,_)), fail.
setRange(X,Y) :- assert(range(X,Y)).
setBarrier(X,Y) :- assert(barrier(X,Y)).

test(X,Y) :- barrier(X,Y), !, fail.
test(X,Y) :- X>0, Y>0, range(RX,RY), X<RX, Y<RY.

nextStep(X,Y,XX,YY) :- XX is X+1, YY is Y, test(XX,YY).
nextStep(X,Y,XX,YY) :- XX is X+1, YY is Y+1, test(XX,YY).
nextStep(X,Y,XX,YY) :- XX is X+1, YY is Y-1, test(XX,YY).
nextStep(X,Y,XX,YY) :- XX is X-1, YY is Y, test(XX,YY).
nextStep(X,Y,XX,YY) :- XX is X-1, YY is Y+1, test(XX,YY).
nextStep(X,Y,XX,YY) :- XX is X-1, YY is Y-1, test(XX,YY).
nextStep(X,Y,XX,YY) :- XX is X, YY is Y+1, test(XX,YY).
nextStep(X,Y,XX,YY) :- XX is X, YY is Y-1, test(XX,YY).


searchPath(_,_,_,_) :- clearPos, fail.
searchPath(X,Y,X,Y) :- assert( pos(X,Y) ).
searchPath(X,Y,X,Y) :- pos(X,Y), retract(pos(X,Y)), !, fail.
searchPath(X,Y,EX,EY) :-
	assert(pos(X,Y)),
	nextStep(X,Y,XX,YY),
	not(pos(XX,YY)),
	searchPath(XX,YY,EX,EY).
searchPath(X,Y,_,_) :-
	pos(X,Y),
	retract(pos(X,Y)),
	fail.

writeP :- not(pos(_,_)),
	write_ln('None'),!.
writeP :-
	pos(X,Y),
	write('Move to:'),
	write(X), write(':'),write_ln(Y),
	fail.
writeP.

clearPos :-
	pos(X,Y),
	retract(pos(X,Y)),
	clearPos, !.
clearPos.
