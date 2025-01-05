#include "Bruno.h"
#include <bits/stdc++.h>
using namespace std;
namespace{
#ifdef zisk
void debug(){cout << endl;}
template<class T, class ... U> void debug(T a, U ... b){cout << a << " ", debug(b...);}
template<class T> void pary(T l, T r) {
	while (l != r) cout << *l << " ", l++;
	cout << endl;
}
#else
#define debug(...) 0
#define pary(...) 0
#endif
#define ll long long
#define maxn 500005
#define pii pair<int, int>
#define ff first
#define ss second
#define io ios_base::sync_with_stdio(0);cin.tie(0);
int N, L;
ll po(ll a, ll n) {
    ll ret = 1;
    while (n) {
        if (n & 1) ret = ret * a;
        n >>= 1;
        a = a * a;
    }
    return ret;
}
vector<int> adj[200000];
vector<int> build_path() {
    int n = 10;
    auto good = [&] (ll x) {
        for (int i = 0;i < n;i++) {
            if (x % 3 == 0) return true;
            x /= 3;
        }
        return false;
    };
    ll m = po(3, n);
    for (int i = 0;i < m;i++) {
        for (int j = 0;j < 3;j++) {
            int to = (i * 3 + j) % m;
            if (!good(i) && j != 0) continue;
            if (i == 0 && j == 0) continue;
            adj[i].push_back(to); 
        }
    } 
    vector<int> path, stk;
    int cur = 0;
    stk.push_back(cur);
    while (stk.size()) {
        cur = stk.back();
        int tmp = cur;
        while (adj[cur].size()) {
            int v = adj[cur].back();
            adj[cur].pop_back();
            cur = v;
            stk.push_back(v);
            break;
        }        
        if (tmp != cur) continue;
        path.push_back(cur);
        stk.pop_back();
    }
    reverse(path.begin(), path.end());
    return path;
}
vector<int> path;
int edge_ind[maxn];

int to_int(char c) {
    if (c == 'R') return 0;
    else if (c == 'G') return 1;
    else return 2;
}
}

void init(int N, int l) {
    ::N = N;
    ::L = l;
    debug("L", l);
    path = build_path();
    for (int i = 1;i < path.size();i++) {
        int val = path[i-1] * 3 + path[i] % 3; 
        assert(edge_ind[val] == 0);
        edge_ind[val] = i;
    }
}

int bruno(string s) {
    if (N == L) return 1;
    assert(s.size() == L);
    vector<int> v[3];
    bool has_zero[3] = {0, 0, 0};
    for (int i = 1;i < L;i++) {
        int a = to_int(s[i]), b = to_int(s[i-1]);
        v[i%3].push_back((a - b + 3) % 3);
        if (a == b) has_zero[i%3] = 1;
    }
    int col = 0;
    for (int i = 0;i < 3;i++) {
        if (has_zero[i] && !has_zero[(i+1)%3]) col = i;
    } 

    assert(has_zero[0] || has_zero[1] || has_zero[2]);
    assert(!has_zero[0] || !has_zero[1] || !has_zero[2]);
    int num = 0;
    for (int i:v[col]) {
        num = num * 3 + i; 
    }
    int pos = -1 + 3 * edge_ind[num]; 
    pos += 1 - col;
    if (col == 0) pos -= 3;
    debug(num, edge_ind[num], col, pos);
    //debug(num, col);
    
    debug("Answer", pos);
    return pos;
}
