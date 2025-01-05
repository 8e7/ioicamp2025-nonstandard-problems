//Challenge: Accepted
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <iomanip>
#include <queue>
#include <unordered_set>
#include <assert.h>
using namespace std;
void debug() {cout << endl;}
template <class T, class ...U> void debug(T a, U ... b) { cout << a << " "; debug(b...);}
template <class T> void pary(T l, T r) {
	while (l != r) {cout << *l << " ";l++;}
	cout << endl;
}
#define ll long long
#define ld long double
#define maxn 1005
#define mod 1000000007
#define pii pair<int, int>
#define ff first
#define ss second
#define io ios_base::sync_with_stdio(0);cin.tie(0);
int n;

int m[maxn];
void label(int l, int r, int ind, int cnt) {
	if (ind < 0) return;
	if (cnt > (r - l) / 2) {
		for (int i = r - cnt - 1;i >= l;i--) m[i] |= (1<<ind);
		int m = (l + r) / 2;
		label(l, m, ind-1, cnt - (r-m+1)/2), label(m, r, ind-1, (r-m+1)/2);
	} else {
		for (int i = l + cnt;i < r;i++) m[i] |= (1<<ind);	
		int m = (l + r) / 2;
		label(l, m, ind-1, cnt - (r-m)/2), label(m, r, ind-1, (r-m)/2);
	}
}
int query(vector<int> a, vector<int> b) {
	cout << "Q " << a.size() << " " << b.size() << " ";
	for (int i:a) cout << i << " ";
	for (int i:b) cout << i << " ";
	cout << endl;
	int res;
	cin >> res;
	return res;
}
int search(vector<int> v, bool type) { //find the different one
	if (v.size() == 1) return v[0];
	int n = v.size() / 2;
	vector<int> c0, c1;
	for (int i = 0;i < n;i++) {
		c0.push_back(v[i]), c1.push_back(v[i+n]);
	}
	int val = query(c0, c1);
	if (val == 0) return v.back();
	else if ((val > 0) == type) {
		return search(c0, type);
	} else {
		return search(c1, type);
	}
}
int main() {
	io
	cin >> n;
	int odd = n % 2;
	n -= odd;
	int bit = 0;
	for (int i = 0;i <= 10;i++) {
		if ((1<<i) >= n) {
			bit = i;
			label(0, n, i-1, n / 2);
			break;
		}
	}
	/*
	pary(m, m + n);
	for (int i = 0;i < bit;i++) {
		int cnt = 0;
		for (int j = 0;j < n;j++) cnt += (m[j] & (1<<i)) ? 1 : 0;
		debug(cnt);
	}
	*/
	int small = 0, big = 0;
	if (odd) {
		vector<int> sp, c0(1, 1), c1(1, 2);
		sp.push_back(n+odd);
		int v0 = query(sp, c0), v1 = query(sp, c1);
		vector<int> num;
		for (int i = 1;i <= n;i++) num.push_back(i);
		if (v0 > 0 && v1 > 0) {
			big = n+odd;	
			small = search(num, 0);
			cout << "A " << small << " " << big << endl;
			return 0;
		} else if (v0 < 0 && v1 < 0) {
			small = n+odd;
			big = search(num, 1);
			cout << "A " << small << " " << big << endl;
			return 0;
		}
	}
	int xo = 0;
	vector<int> vs;
	for (int i = 0;i < bit;i++) {
		vector<int> c0, c1;
		for (int j = 0;j < n;j++) {
			if (m[j] & (1<<i)) c1.push_back(j+1);	
			else c0.push_back(j+1);
		}
		assert(c0.size() == c1.size());
		int val = query(c0, c1);
		if (val > 0) xo += 1<<i, vs = c1;
		else if (val < 0) xo += 1<<i, vs = c0;
	}
	small = search(vs, 0);	
	for (int i = 0;i < n;i++) {
		if (m[i] == (m[small-1] ^ xo)) {
			big = i+1;
			break;
		}
	}
	
	
	cout << "A " << small << " " << big << endl;
	return 0;
}


