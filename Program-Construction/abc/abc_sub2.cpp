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
	vector<pair<string, int> > a(n);
	for (int i = 0;i < n;i++) {
		a[i].ff = names[i];
		a[i].ss = i;
	}
	sort(a.begin(), a.end());
	int ind = 0;
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < NUM_LEN;j++) {
			outputs_alice[ind++] = (numbers[a[i].ss]>>j)&1;
		}
	}
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) {
			if (a[i].ss == j) outputs_alice[ind++] = 1;
			else outputs_alice[ind++] = 0;
		}
	}
    return ind;
}


// Bob
int // returns lb
bob(
    /*  in */ const int m,
    /*  in */ const char senders[][5],
    /*  in */ const char recipients[][5],
    /* out */ bool outputs_bob[]
) {
	vector<string> names;
	for (int i = 0;i < m;i++) {
		names.push_back(senders[i]);
		names.push_back(recipients[i]);
	}
	sort(names.begin(), names.end());
	names.resize(int(unique(names.begin(), names.end()) - names.begin()));
	int n = names.size();
	for (int i = 0;i < n*n;i++) outputs_bob[i] = 0;
	for (int i = 0;i < m;i++) {
		int x = lower_bound(names.begin(), names.end(), senders[i]) - names.begin();
		int y = lower_bound(names.begin(), names.end(), recipients[i]) - names.begin();
		outputs_bob[x*n+y] = 1;
	}
    return n*n;
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
	int n = sqrt(lb);
	if (la == 0) return cur_op;
	auto op = [&] (int type, int a, int b) {
		operations[cur_op] = type;	
		operands[cur_op][0] = a; operands[cur_op][1] = b;
		return cur_op++;
	};
	auto mask = [&] (int a, int s) {
		int ret = cur_op;
		for (int i = 0;i < NUM_LEN;i++) op(OP_AND, a+i, s);	
		return ret;
	};

	int zero = cur_op;
	for (int i = 0;i < NUM_LEN;i++) op(OP_ZERO, 0, 0);
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

	vector<int> ans(n, zero), nums(n);
	int grid_a = n*NUM_LEN, grid_b = la;
	for (int i = 0;i < n;i++) nums[i] = i*NUM_LEN;
	for (int i = 0;i < n;i++) { //send
		for (int j = 0;j < n;j++) { //rec
			int val = mask(nums[i], grid_b + i*n+j);
			ans[j] = add(ans[j], val);
		}
	}

	//permute ans
	vector<int> out(n, zero);
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) {
			int val = mask(ans[i], grid_a + i*n+j);
			int upd = cur_op;
			for (int k = 0;k < NUM_LEN;k++) op(OP_OR, out[j]+k, val+k);
			out[j] = upd;
		}
	}
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < 16;j++) outputs_circuit[i][j] = out[i]+j;
	}
	
    return cur_op;
}
