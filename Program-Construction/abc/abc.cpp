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

const int NAME_LEN = 19;
const int NUM_LEN = 16;
const int OBJ_LEN = 2*NAME_LEN + NUM_LEN + 1;
std::vector<int> deb; //debugging indices

int name_to_num(const char c[]) {
	int val = 0, end = 0;
	for (int i = 0;i < 4;i++) {
		if (i == 0) val = (c[i] - 'a');
		else {
			if (c[i] == '\0') end = 1;
			if (!end) val = val * 27 + (c[i] - 'a' + 1);
			else val = val * 27;
		}
	}
	return val;
}
struct obj{
	int u, v, w, t;
	obj(){u = v = w = t = 0;}
	obj(int _u, int _v, int _w, int _t) {
		u = _u, v = _v, w = _w, t = _t;
	}
};
void print(bool b[], int p, int len) {
	//prints number in b[p] to b[p+len-1], low bit first
	int val = 0;
	for (int i = p;i < p+len;i++) val += (1<<(i-p)) * (b[i]);
	cout << val << " ";
}
void printobj(bool b[], int p) {
	print(b, p, NAME_LEN); cout << " ";
	print(b, p+NAME_LEN, NAME_LEN); cout << " ";
	print(b, p+2*NAME_LEN, NUM_LEN); cout << " ";
	print(b, p+2*NAME_LEN+NUM_LEN, 1); debug();
}
void write(int num, int len, bool b[], int &ind) {
	//writes num in b starting from ind, len bits
	for (int i = 0;i < len;i++) {
		b[ind++] = num&1;
		num >>= 1;	
	}
}
void write(vector<bool> &v, bool b[], int &ind) {
	//writes num in b starting from ind, len bits
	int len = v.size();
	for (int i = 0;i < len;i++) b[ind++] = v[i];
}
void write(obj o, bool b[], int &ind) {
	for (int i = 0;i < NAME_LEN;i++) b[ind++] = (o.u>>i)&1;
	for (int i = 0;i < NAME_LEN;i++) b[ind++] = (o.v>>i)&1;
	for (int i = 0;i < NUM_LEN;i++) b[ind++] = (o.w>>i)&1;
	b[ind++] = o.t&1;
}
void get_perm(vector<int> p, vector<bool> &v) {
	//gets list of swaps for perm (0 base)
	int n = p.size();
	if (n <= 1) return;	
	bool odd = 0;
	if (n % 2) {
		odd = 1;
		p.push_back(n);
		n++;
	}
	int m = n / 2;
	vector<int> pos(n);
	vector<bool> vis(m, 0);
	vector<bool> swaps(m, 0);
	for (int i = 0;i < n;i++) pos[p[i]] = i;
	auto other = [&] (int x) {
		return x<m ? x+m : x-m;
	};
	auto flip = [&] (int i) {
		swaps[i] = 1 - swaps[i];
		swap(p[i], p[i+m]);
		pos[p[i]] = i;
		pos[p[i+m]] = i+m;
	};
	for (int i = 0;i < m;i++) {
		if (vis[p[i]%m]) continue;
		int cur = p[i];
		do {
			if ((pos[cur]<m) == (pos[other(cur)] < m)) {
				flip(pos[cur]%m);
			}
			vis[cur%m] = 1;
			cur = other(p[other(pos[cur])]);
		} while (!vis[cur%m]);
	}
	if (pos[n-1] < m) {
		for (int i = 0;i < m;i++) flip(i);
	}
	v.insert(v.end(), swaps.begin(), swaps.end());	
	vector<int> pl(p.begin(), p.begin() + m), pr(p.begin()+m, p.begin()+n-odd);
	for (int &i:pl) i = (i < m ? i : i-m);
	for (int &i:pr) i = (i < m ? i : i-m);
	get_perm(pl, v);
	get_perm(pr, v);
	for (int i = 0;i < m;i++) {
		if (pos[i] >= m) v.push_back(1);
		else v.push_back(0);
	}
}

// Alice
int // returns la
alice(
    /*  in */ const int n,
    /*  in */ const char names[][5],
    /*  in */ const unsigned short numbers[],
    /* out */ bool outputs_alice[]
) {
	vector<pair<int, int> > vec;
	for (int i = 0;i < n;i++) {
		vec.push_back(make_pair(name_to_num(names[i]), i));
	}
	sort(vec.begin(), vec.end());
	int ind = 0;
	for (int i = 0;i < n;i++) {
		obj o(vec[i].ff, vec[i].ff, (int)numbers[vec[i].ss], 0);
		write(o, outputs_alice, ind);
	}
	vector<int> perm(n); //sorted by U -> input order
	vector<bool> ps;
	for (int i = 0;i < n;i++) perm[i] = vec[i].ss;	
	pary(perm.begin(), perm.end());
	get_perm(perm, ps);	
	write(ps, outputs_alice, ind);	
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
	vector<pair<obj, int> > v(m);	
	for (int i = 0;i < m;i++) {
		v[i].ff = obj(name_to_num(senders[i]), name_to_num(recipients[i]), 0, 1);
	}
	sort(v.begin(), v.end(), [&] (auto p, auto q) { return p.ff.v == q.ff.v ? p.ff.u < q.ff.u : p.ff.v > q.ff.v;});
	for (int i = 0;i < m;i++) v[i].ss = i;
	sort(v.begin(), v.end(), [&] (auto p, auto q) { return p.ff.u == q.ff.u ? p.ff.v < q.ff.v : p.ff.u > q.ff.u;});
	int ind = 0;
	for (int i = 0;i < m;i++) {
		write(v[i].ff, outputs_bob, ind);
	}
	vector<int> perm(m); //sorted by u -> sorted by v
	vector<bool> ps;
	for (int i = 0;i < m;i++) perm[i] = v[i].ss;
	get_perm(perm, ps);	
	write(ps, outputs_bob, ind);	
    return ind;
}

int perm_size(int l, int r) {
	if (r - l <= 1) return 0;
	int n = r - l;
	int m = (n + 1) / 2;
	return 2 * m + perm_size(l, l+m) + perm_size(l+m, r);
}
int get_size(int s) {
	//finds an n such that OBJ_LEN*n + perm_size(n) == s
	int low = 0, up = 1001;
	while (low < up - 1) {
		int mid = (low + up) / 2;
		if (mid * OBJ_LEN + perm_size(0, mid) <= s) low = mid;
		else up = mid;	
	}
	return low;
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
	auto ops = [&] (int type, int a, int b, int len) {
		int ret = cur_op;
		for (int i = 0;i < len;i++) op(type, a+i, b+i);
		return ret;
	};
	auto mask = [&] (int x, int s, int flip, int len) { //masks x by s^flip
		int ret = cur_op;
		for (int i = 0;i < len;i++) {
			if (flip) op(OP_LESS, s, x+i);
			else op(OP_AND, s, x+i);
		}
		return ret;
	};

	auto set_var = [&] (int len, int b) { //initialize variable
		int ret = cur_op;
		for (int i = 0;i < len;i++) op((b ? OP_ONE : OP_ZERO), 0, 0);
		return ret;
	};	
	int inf = set_var(OBJ_LEN, 1);
	int zero = set_var(OBJ_LEN, 0);

	auto tristate = [&] (int s, int a, int b, int len) { //s=0 for a, s=1 for b
		vector<int> ga(len), gb(len);
		for (int i = 0;i < len;i++) {
			ga[i] = op(OP_LESS, s, a+i);
			gb[i] = op(OP_AND, s, b+i);
		}
		int ret = cur_op;
		for (int i = 0;i < len;i++) op(OP_OR, ga[i], gb[i]);
		return ret;
	};
	auto swapobj = [&] (int &p, int &q, int s) {
		//swaps p and q if s == 1
		int np, nq;
		np = tristate(s, p, q, OBJ_LEN);
		int tmp = cur_op;
		ops(OP_XOR, p, q, OBJ_LEN);
		nq = cur_op;
		ops(OP_XOR, np, tmp, OBJ_LEN);
		p = np, q = nq;
		return tmp;
	};
	auto reorder = [&] (int &p, int &q, vector<int> &comp) {
		//reorders such that p <= q by comp, and returns whether there is a swap
		//comp starts from least significant bit
		if (q == inf) return zero;
		if (p == inf) {
			swap(p, q);
			return inf;
		}
		int prevb = 0;
		for (int i = 0;i < comp.size();i++) {
			int b = comp[i];
			int ts = op(OP_LESS, p+b, q+b), tb = op(OP_GREATER, p+b, q+b);
			prevb = op(OP_GREATER, prevb, ts); //0010
			prevb = op(OP_OR, prevb, tb); //0111
		}
		swapobj(p, q, prevb);
		return prevb;
	};

	vector<int> swaplist;
	vector<pii> swappairs;
	auto merge = [&] (auto merge1, vector<int> &v, int l, int r, vector<int> &comp) {
		//performs merge on bitonic array v
		//assumes size is 2^k	
		if (r - l <= 1) return;
		int m = (l + r) / 2;
		for (int i = l;i < m;i++) {
			swaplist.push_back(reorder(v[i], v[i + m-l], comp));	
			swappairs.push_back({i, i+m-l});
		}
		merge1(merge1, v, l, m, comp);
		merge1(merge1, v, m, r, comp);
	};	
	auto undo_merge = [&] (vector<int> &v) {
		int siz = swappairs.size();
		for (int i = siz-1;i >= 0;i--) {
			swapobj(v[swappairs[i].ff], v[swappairs[i].ss], swaplist[i]);
		}	
		swaplist.clear();
		swappairs.clear();
	};

	//adder
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

	//permutation
	auto apply_perm = [&] (auto apply_perm1, vector<int> &p, int l, int r, int &cur) {
		if (r - l <= 1) return;
		int n = r - l;
		int m = (n+1) / 2;
		for (int i = l;i < l+m;i++, cur++) {
			if (i+m<r)swapobj(p[i], p[i+m], cur);
		}
		apply_perm1(apply_perm1, p, l, l+m, cur);
		apply_perm1(apply_perm1, p, l+m, r, cur);
		for (int i = l;i < l+m;i++, cur++) {
			if(i+m<r)swapobj(p[i], p[i+m], cur);
		}
	};
	
	int n = get_size(la), m = get_size(lb);
	int sort_size = 1;
	while (sort_size < n+m) sort_size<<=1;
	vector<int> objs(sort_size, inf); //list of (pointers to) objects
	for (int i = 0;i < n;i++) objs[i] = i * OBJ_LEN;
	for (int i = 0;i < m;i++) objs[sort_size-m+i] = la + i * OBJ_LEN;


	//Part 1: Stores values of letters <- Number of senders
	vector<int> UT = {OBJ_LEN-1}; //sort first U then T
	for (int i = 0;i < NAME_LEN;i++) UT.push_back(i);
	merge(merge, objs, 0, sort_size, UT);
	//objs[0~n+m-1] is in order
	int C = zero; //tmp variable
	for (int i = 0;i < n+m;i++) {
		C = mask(C, objs[i]+OBJ_LEN-1, 0, NUM_LEN);
		int tmp = objs[i] + 2*NAME_LEN;	
		tmp = mask(tmp, objs[i] + OBJ_LEN-1, 1, NUM_LEN);
		
		int upd = mask(objs[i], inf, 0, 2*NAME_LEN);
		ops(OP_OR, objs[i] + 2*NAME_LEN, C, NUM_LEN);
		op(OP_NOT_X0, objs[i]+OBJ_LEN-1, objs[i]+OBJ_LEN-1); //flips T
		objs[i] = upd;

		C = ops(OP_OR, C, tmp, NUM_LEN);
	}
	undo_merge(objs);
	{
		int tmp = la+m*OBJ_LEN;
		apply_perm(apply_perm, objs, sort_size-m, sort_size, tmp);
	}

	//Part 2: Adds sum to recepients
	vector<int> VT = {OBJ_LEN-1};
	for (int i = 0;i < NAME_LEN;i++) VT.push_back(i+NAME_LEN);
	merge(merge, objs, 0, sort_size, VT);
	
	C = zero;	
	for (int i = 0;i < n+m;i++) {
		int tmp = objs[i] + 2*NAME_LEN;	
		tmp = mask(tmp, objs[i] + OBJ_LEN-1, 1, NUM_LEN);
		C = add(C, tmp);
		
		int upd = mask(objs[i], inf, 0, 2*NAME_LEN);
		mask(C, objs[i] + OBJ_LEN-1, 0, NUM_LEN);
		op(OP_X0, objs[i]+OBJ_LEN-1, objs[i]+OBJ_LEN-1);
		objs[i] = upd;

		C = mask(C, objs[i]+OBJ_LEN-1, 1, NUM_LEN);
	}
	undo_merge(objs);
	{
		int tmp = n*OBJ_LEN;
		apply_perm(apply_perm, objs, 0, n, tmp);
	}
	
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < 16;j++) outputs_circuit[i][j] = objs[i] + 2*NAME_LEN+j;
	}
	
    return cur_op;
}
