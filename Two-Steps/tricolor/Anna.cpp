#include "Anna.h"
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
#define maxn 200005
#define pii pair<int, int>
#define ff first
#define ss second
#define io ios_base::sync_with_stdio(0);cin.tie(0);


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

int to_int(char c) {
    if (c == 'R') return 0;
    else if (c == 'G') return 1;
    else return 2;
}

char to_char(int i) {
    const string S = "RGB";
    return S[i];
}

int add_diff(int prv, int res) {
    if ((prv+1)%3 != res) return (prv+1)%3;
    else return (prv+2)%3;
}
void add_bit(std::vector<int> &ret, std::vector<int> &v, int bit) {
    //add semantic bit, could be 0/1/2, which takes up v[ind] and v[ind+1]
    int ind = ret.size();
    if (ind >= v.size()) return;
    if (ind + 1 >= v.size()) {
        ret.push_back(add_diff(ret.back(), v[ind]));
        return;
    }
    int done = 0;
    for (int i = 1;i < 3;i++) {
        for (int j = 1;j < 3;j++) {
            int vi = (v[ind] + i) % 3, vj = (v[ind+1] + j) % 3;
            if ((vj - vi + 3) % 3 == bit) {
                ret.push_back(vi);
                ret.push_back(vj);
                done = 1;
                break;
            }
        }
        if (done) break;
    } 
    ind = ret.size();
    if (ind >= v.size()) return;
    ret.push_back(add_diff(ret.back(), v[ind]));
}
}

pair<string, int> anna(int N, string S) {
    vector<int> v(N, 0), ret;
    for (int i = 0;i < N;i++) v[i] = to_int(S[i]);
    ret.push_back((v[0] + 1) %3);
    vector<int> nodes = build_path(); 
    vector<int> dig;
    for (int i = 0;i < 10;i++) add_bit(ret, v, 0), dig.push_back(0);
    for (int i = 1;i < nodes.size();i++) {
        if (ret.size() >= N) break;
        add_bit(ret, v, nodes[i] % 3);
        dig.push_back(nodes[i] % 3);
    } 
    pary(dig.begin(), dig.end());
    while (ret.size() >= N) ret.pop_back(); 
    
    int L = min(N, 34);
    string T;
    for (int i = 0;i < N;i++) T.push_back(to_char(ret[i]));
    return make_pair(T, L);
}

