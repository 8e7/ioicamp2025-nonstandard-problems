#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cassert>

// Functions to be implemented in the solution.
std::vector<std::pair<int, int>> recover_tunnels(int N);

int get_total_distances(const std::vector<int> &subset);

namespace {
    int N, lgN;
    long long query_count, total_count;
    std::vector<int> dfs_order, pos, dep, subtree, vis;
    std::vector<std::vector<int>> graph, vgraph, table;
    int min_node(int a, int b) {
        if (dep[a] < dep[b]) return a;
        return b;
    }
    void dfs(int u, int f, int d) {
        dep[u] = d;
        pos[u] = dfs_order.size();
        dfs_order.push_back(u);
        for (int v : graph[u])
            if (v != f) {
                dfs(v, u, d + 1);
                dfs_order.push_back(u);
            }
    }
    void init(int _N, std::vector<std::pair<int, int>> edges) {
        N = _N, query_count = 0, total_count = 0;
        graph.resize(N + 1), vgraph.resize(N + 1), vis.resize(N + 1);
        for (auto [u, v] : edges) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        pos.resize(N + 1), dep.resize(N + 1), subtree.resize(N + 1);
        dfs_order.clear(), dfs(1, 1, 0);
       
        lgN = std::__lg(dfs_order.size());
        std::vector<std::vector<int>>(lgN + 1, dfs_order).swap(table);
        for (int i = 1; i <= lgN; i++)
            for (int j = 0; j + (1 << i) <= int(dfs_order.size()); j++)
                table[i][j] = min_node(table[i - 1][j], table[i - 1][j + (1 << (i - 1))]);
    }
    int LCA(int u, int v) {
        int l = pos[u], r = pos[v];
        if (l > r) std::swap(l, r);
        int lg = std::__lg(r - l + 1);
        return min_node(table[lg][l], table[lg][r - (1 << lg) + 1]);
    }
    int build_induced_tree(std::vector<int> subset) { // return the root node of the induced tree
        std::sort(subset.begin(), subset.end(), [&](int u, int v) {
            return pos[u] < pos[v];
        });
        std::vector<int> vnodes(subset);
        for (int i = 0; i + 1 < int(subset.size()); i++) {
            vnodes.push_back(LCA(subset[i], subset[i + 1]));
        }
        std::sort(vnodes.begin(), vnodes.end(), [&](int u, int v) {
            return pos[u] < pos[v];
        });

        vnodes.resize(std::unique(vnodes.begin(), vnodes.end()) - vnodes.begin());
        for (int u : vnodes) {
            vgraph[u].clear();
            subtree[u] = 0;
        }
        for (int u : subset) {
            subtree[u] = 1;
        }
        for (int i = 0; i + 1 < int(vnodes.size()); i++) {
            int lca = LCA(vnodes[i], vnodes[i + 1]);
            vgraph[lca].push_back(vnodes[i + 1]);  
        }

        return vnodes[0];
    }
    int cal_distance(const std::vector<int> &subset) {
        int sz = subset.size();
        int root = build_induced_tree(subset);
        int res = 0;
        
        auto cal = [&](auto _cal, int u) -> void {
            for (int v : vgraph[u]) {
                _cal(_cal, v);
                subtree[u] += subtree[v];
                res += subtree[v] * (sz - subtree[v]) * (dep[v] - dep[u]);
            }
        };

        cal(cal, root);
        return res;
    }
    void wrong_answer(const std::string &msg) {
        printf("Wrong Answer: %s\n", msg.c_str());
        exit(0);
    }
}

int get_total_distances(const std::vector<int> &subset) {
    if (subset.empty()) 
        wrong_answer("Empty subset");
    query_count += 1;
    total_count += subset.size();
    static int vis_count = 0;
    vis_count += 1;
    for (int u : subset) {
        if (u < 1 || u > N) {
            wrong_answer("Invalid vertex number: " + std::to_string(u));
        }
        if (vis[u] == vis_count) {
            wrong_answer("Duplicate vertex numbers: " + std::to_string(u));
        }
        vis[u] = vis_count;
    }
    return cal_distance(subset);
}

int main() {
    int N;
    assert(1 == scanf("%d", &N));
    std::vector<std::pair<int, int>> edges(N - 1);
    for (auto &[u, v] : edges) {
        assert(2 == scanf("%d %d", &u, &v));
    }
    init(N, edges);
    auto res = recover_tunnels(N);

    if (int(res.size()) != N - 1)
        wrong_answer("Incorrect number of edges");

    auto minimum_edges = [&](std::vector<std::pair<int, int>> tree) {
        for (auto &[u, v] : tree) {
            if (u > v)
                std::swap(u, v);
        }
        std::sort(tree.begin(), tree.end());
        return tree;
    };

    for (auto &[u, v] : res)
        printf("%d %d\n", u, v);

    if (minimum_edges(res) != minimum_edges(edges))
        wrong_answer("Incorrect tree");
    
    printf("Accepted: %lld queries\n", query_count);

    return 0;
}
