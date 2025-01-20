//Challenge: Accepted
#include <bits/stdc++.h>
#define maxn 1505
#define pii pair<int, int>
#define ff first
#define ss second
#define ll long long
using namespace std;
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

// Functions provided to the solution.
int get_total_distances(const std::vector<int> &subset);

vector<int> adj[maxn];
vector<pair<int, int>> return_edges;
vector<int> nodes;

void add_edge(int u, int v) { //must be par[v], v
    return_edges.push_back({u, v});
    adj[u].push_back(v);
    adj[v].push_back(u);
    nodes.push_back(v);
}

bool valid[maxn];
int cnt[maxn], dep[maxn];
void get_count(int n, int par, int d){
    //DFS from the tree to get cnt for each node
    cnt[n] = valid[n];
    dep[n] = d;
    for (int v:adj[n]) {
        if (v != par) {
            get_count(v, n, d+1);
            cnt[n] += cnt[v];
        }
    }
}

void get_S(int n, int par, int tot, vector<int> &S, int &chi) {
    //DFS from the tree to obtain the set S for querying.
    //S is a path from root to a node with cnt[n] >= |D_i|/2,
    //and chi is the number of children from n that are chosen
    S.push_back(n);
    vector<pii> c;
    for (int v:adj[n]) {
        if (v != par) {
            c.push_back({cnt[v], v});
            if (2 * cnt[v] >= tot) {
                get_S(v, n, tot, S, chi);
                return;
            }
        }
    }
    //n is the final node, select some children of n
    sort(c.begin(), c.end(), [&] (pii x, pii y){return x.ff > y.ff;}); 
    int best = 0, bind = -1;
    int sum = 0;
    for (int i = 0;i < c.size();i++) {
        sum += c[i].ff;
        if (min(sum, cnt[n] - sum) > best) {
            best = min(sum, cnt[n] - sum);
            bind = i;
        }
    }
    chi = bind+1;
    for (int i = 0;i <= bind;i++) {
        S.push_back(c[i].ss);
    }
    return;
}
void get_cand(int n, int par,vector<int> &cand) {
    //push possible candidates to vector cand
    if (valid[n]) {
        cand.push_back(n);
        return;
    }
    for (int v:adj[n]) {
        if (v != par) {
            get_cand(v, n, cand);
        }
    }
}
int f(int i, int j, int k, int child) {
    //query result of S U {x}, where x has depth i
    //LCA of {x} and v/child of v is depth j, v has depth k,
    //and child children of v are selected
    int base = (ll) k * (k+1) * (k+1) / 2 - (ll) k * (k+1) * (2*k+1) / 6 + child * (child-1) + (ll) child * (k+1) * (k+2)/2;
    if (j <= k) {
        base +=(i + i - j) * (j+1) / 2 + (i-j+1 + i-j + k-j) * (k-j)/2 + child * (i-j+k-j+1); 
    } else {
        base +=(i + i - k-1) * (k+2)/2 + (child-1)*(i-k+1);
    }
    return base;
}

bool inpath[maxn];
void solve(int n, int depth, vector<int> cand, vector<int> query) {
    if (query.size() == 0) return;
    if (cand.size() == 1) {
        for (int i:query) {
            add_edge(cand[0], i);
        }
        return;
    }
    //cand is D_i, query is D_{i+1}
    for (int i = 0;i <= n;i++) valid[i] = 0; 
    for (int i:cand) valid[i] = 1;
    get_count(1, 0, 0);

    vector<int> S;
    int chi = 0;
    get_S(1, 0, cnt[1], S, chi);
    
    int len = S.size() - chi + 1, vdep = S.size()-chi-1; 
    unordered_map<int, int> values;
    for (int i = 0;i < len;i++) {
        values[f(depth, i, vdep, chi)] = i;
    }
    vector<vector<int>> res(len);
    for (int x:query) {
        vector<int> T = S;
        T.push_back(x);
        int val = get_total_distances(T);
        res[values[val]].push_back(x);
    }
    for (int i:S) inpath[i] = 1; 
    vector<vector<int>> new_cand(len);
    for (int i:S) {
        if (dep[i] == depth-1) new_cand[dep[i]].push_back(i);
        else {
            for (int v:adj[i]) {
                if (!inpath[v] && dep[v] > dep[i]) {
                    get_cand(v, i, new_cand[dep[i]]);
                }
            }
        }
    }
    for (int i:S) inpath[i] = 0; 
    for (int i = 0;i < len;i++) solve(n, depth, new_cand[i], res[i]);
}

std::vector<std::pair<int, int>> recover_tunnels(int N) {
    nodes.push_back(1);
    vector<vector<int>> D(N, vector<int>());
    for (int i = 2; i <= N; i++) {
        vector<int> S = {1, i};
        int dep = get_total_distances(S);
        D[dep].push_back(i); 
    }
    for (int i:D[1]) add_edge(1, i); 
    for (int i = 1;i < N-1;i++) {
        if (D[i+1].size() == 0) break;
        solve(N, i+1, D[i], D[i+1]);
    }
    return return_edges;
}
