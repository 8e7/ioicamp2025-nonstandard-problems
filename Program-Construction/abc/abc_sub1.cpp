#include "abc.h"
#include <bits/stdc++.h>
using namespace std;
void debug(){cerr << endl;}
template<class T, class ... U> void debug(T a, U ... b){cerr << a << " ", debug(b...);}
template<class T> void pary(T l, T r) {
	while (l != r) cerr << *l << " ", l++;
	cerr << endl;
}
#define ll long long
#define pii pair<int, int>
#define ff first
#define ss second

// you may find the definitions useful
const int OP_ZERO    = 0;  // f(OP_ZERO,    x0, x1) = 0
const int OP_NOR     = 1;  // f(OP_NOR,     x0, x1) = !(x0 || x1)
const int OP_GREATER = 2;  // f(OP_GREATER, x0, x1) = (x0 > x1)
const int OP_NOT_X1  = 3;  // f(OP_NOT_X1,  x0, x1) = !x1
const int OP_LESS    = 4;  // f(OP_LESS,    x0, x1) = (x0 < x1)
const int OP_NOT_X0  = 5;  // f(OP_NOT_X0,  x0, x1) = !x0
const int OP_XOR     = 6;  // f(OP_XOR,     x0, x1) = (x0 ^ x1)
const int OP_NAND    = 7;  // f(OP_NAND,    x0, x1) = !(x0 && x1)
const int OP_AND     = 8;  // f(OP_AND,     x0, x1) = (x0 && x1)
const int OP_EQUAL   = 9;  // f(OP_EQUAL,   x0, x1) = (x0 == x1)
const int OP_X0      = 10; // f(OP_X0,      x0, x1) = x0
const int OP_GEQ     = 11; // f(OP_GEQ,     x0, x1) = (x0 >= x1)
const int OP_X1      = 12; // f(OP_X1,      x0, x1) = x1
const int OP_LEQ     = 13; // f(OP_LEQ,     x0, x1) = (x0 <= x1)
const int OP_OR      = 14; // f(OP_OR,      x0, x1) = (x0 || x1)
const int OP_ONE     = 15; // f(OP_ONE,     x0, x1) = 1

const int NUM_LEN = 16;
// Alice
int // returns la
alice(
    /*  in */ const int n,
    /*  in */ const char names[][5],
    /*  in */ const unsigned short numbers[],
    /* out */ bool outputs_alice[]
) {
	for (int i = 0;i < NUM_LEN;i++) {
		outputs_alice[i] = (numbers[0]>>i)&1;
	}
    return NUM_LEN;
}


// Bob
int // returns lb
bob(
    /*  in */ const int m,
    /*  in */ const char senders[][5],
    /*  in */ const char recipients[][5],
    /* out */ bool outputs_bob[]
) {
	for (int i = 0;i < NUM_LEN;i++) {
		outputs_bob[i] = (m>>i)&1;
	}
    return NUM_LEN;
}

// Circuit
int // returns l
circuit(
    /*  in */ const int la,
    /*  in */ const int lb,
    /* out */ int operations[],
    /* out */ int operands[][2],
    /* out */ int outputs_circuit[][16]
) {
	int cur_op = la+lb;
	if (la == 0) return cur_op;
	auto op = [&] (int type, int a, int b) {
		operations[cur_op] = type;	
		operands[cur_op][0] = a; operands[cur_op][1] = b;
		return cur_op++;
	};
	int zero = op(OP_ZERO, 0, 0);
	auto add = [&] (int x, int y) { //adds x and y (size NUM_LEN)
		vector<int> cins = {zero};
		vector<int> gs;
		for (int i = 0;i < NUM_LEN;i++) {
			int g1 = op(OP_XOR, x+i, y+i);
			int c1 = op(OP_AND, x+i, y+i);
			int c2 = op(OP_AND, cins.back(), g1);
			if (i < NUM_LEN-1) cins.push_back(op(OP_OR, c1, c2));
			gs.push_back(g1);
		}
		int ret = cur_op;
		for (int i = 0;i < NUM_LEN;i++) op(OP_XOR, cins[i], gs[i]);
		return ret;
	};
	int ans = cur_op;
	for (int i = 0;i < NUM_LEN;i++) op(OP_ZERO, 0, 0);
	int p = 0, m = NUM_LEN;
	for (int i = 0;i < 10;i++) {
		int p2 = cur_op;
		for (int j = 0;j < NUM_LEN;j++) op(OP_AND, m+i, p+j);
		ans = add(ans, p2);	
		p = add(p, p);
	}

	for (int j = 0;j < 16;j++) outputs_circuit[0][j] = ans+j;
	
    return cur_op;
}
