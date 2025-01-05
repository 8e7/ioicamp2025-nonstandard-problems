//Challenge: Accepted
#include <bits/stdc++.h>
#pragma GCC optimize("Ofast")
using namespace std;
#ifdef zisk
void debug(){cout << endl;}
template<class T, class ... U> void debug(T a, U ... b){cerr << a << " ", debug(b...);}
template<class T> void pary(T l, T r) {
	while (l != r) cerr << *l << " ", l++;
	cerr << endl;
}
#else
#define debug(...) 0
#define pary(...) 0
#endif
#define ll long long
#define maxn 1024
#define mod 1000000007
#define pii pair<int, int>
#define ff first
#define ss second
#define io ios_base::sync_with_stdio(0);cin.tie(0);
int n, m, K;
int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
string cell_type = "X#.";
bool in(int x, int y) {return x >= 0 && x < n && y >= 0 && y < m;}
vector<vector<int> > read_grid(string file) {
	vector<vector<int> > res;
	fstream cin(file);
	if (!cin.good()) return res; //if file doesn't exist
	string str;
	while (getline(cin, str)) {
		int _m = str.size();	
		vector<int> tmp(_m);
		for (int i = 0;i < _m;i++) {
			if (str[i] == '.') tmp[i] = 2;
			else if (str[i] == '#') tmp[i] = 1;
			else tmp[i] = 0;
		}
		res.push_back(tmp);
	}	
	return res;
}
int eval(vector<vector<int> > &grid) {
	int ret = 0;
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < m;j++) {
			if (grid[i][j] == 2) {
				int deg = 0;
				for (int k = 0;k < 4;k++) {
					int ni = i + dir[k][0], nj = j + dir[k][1];
					if (in(ni, nj) && grid[ni][nj] == 2) deg++;
				}
				if (deg == 1) ret++;
			}	
		}
	}	
	return ret;
}
void expand(int sx, int sy, vector<vector<int> > &grid) {
	priority_queue<pair<int, pii> > pq;
	auto get_deg = [&] (int x, int y) {
		int deg = 0;
		int sd = 0;
		for (int k = 0;k < 4;k++) {
			int nx = x + dir[k][0], ny = y + dir[k][1];
			if (in(nx, ny) && grid[nx][ny] == 2) sd++;
			if (in(nx, ny) && grid[nx][ny] == 0) {
				bool ok = 1;
				for (int l = 0;l < 4;l++) {
					int px = nx + dir[l][0], py = ny + dir[l][1];
					if (in(px, py) && grid[px][py] == 2) {
						ok = 0;
						break;
					}
				}
				if (ok) deg++;
			}
		}
		if (sd>1) return -1;
		return deg;
	};
	auto add_elem = [&] (int x, int y) {
		if (!in(x, y) || grid[x][y] != 0) return;
		int d = get_deg(x, y);
		if (d == -1) return;
		pq.push(make_pair(d, make_pair(x, y)));
	};
	pq.push(make_pair(get_deg(sx, sy), make_pair(sx, sy)));
	while (pq.size()) {
		auto [deg, pnt] = pq.top();pq.pop();
		auto [x, y] = pnt;
		if (get_deg(x, y) != deg) continue;
		grid[x][y] = 2; //mark as visited
		//debug(x, y);
		for (int k = 0;k < 4;k++) {
			int nx = x + dir[k][0], ny = y + dir[k][1];
			if (in(nx, ny) && grid[nx][ny] == 0) {
				int cnt = 0;
				for (int l = 0;l < 4;l++) {
					int px = nx + dir[l][0], py = ny + dir[l][1];
					if (in(px, py) && grid[px][py] == 2) cnt++;
				}
				//debug("dir", k, cnt);
				if (cnt == 1) {
					grid[nx][ny] = 2;
					for (int l = 0;l < 4;l++) {
						int px = nx + dir[l][0], py = ny + dir[l][1];
						add_elem(px, py);
					}
				}
			}
		}
	}
}
int main() {
	int testcase;
	std::cin >> testcase;
	string filename = "nowruz" + to_string(testcase);
	string infile = filename + ".in.txt";
	string outfile = filename + ".out.txt";
	
	fstream cin(infile);
	fstream cout(outfile);
	if (cout.bad()) {
		cout.open(outfile, ios::out);	
	}
	cin >> n >> m >> K;
	
	vector<vector<int> > grid(n, vector<int>(m, 0));
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < m;j++) {
			char c;
			cin >> c;
			if (c == '.') grid[i][j] = 0;	
			else grid[i][j] = 1;
		}	
	}
	
	vector<vector<int> > best = read_grid(outfile);
	int bval = -1;
	srand(time(NULL));
	int trials = 1000;
	for (int trial = 0;trial < trials;trial++) {
		int sx, sy;
		do {
			sx = rand() % n, sy = rand() % m;	
		} while (grid[sx][sy] != 0);
		vector<vector<int> > solve = grid;	
		expand(sx, sy, solve);
		//for (int i = 0;i < n;i++) pary(solve[i].begin(), solve[i].end());
		int val = eval(solve);
		if (val > bval) {
			best = solve, bval = val;
		}
		if (val >= K) break;
	}	
	//write to ans;	
	debug("found", bval, double(bval) / K);
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < m;j++) cout << cell_type[best[i][j]];
		cout << "\n";
	}
}
