#include "Bruno.h"
#include <vector>
#include <iostream>
using namespace std;
namespace {
 
int inp[3][3];
int px[9], py[9];
int getans(int dx, int dy) {
	int ret = 0;
	if (dx == 0 && dy == 0) return 4;
	if (dy > 0) ret = 0;
	else if (dy < 0) ret = 1;
	else if (dx > 0) ret = 2;
	else ret = 3;
	return ret;
}
} // namespace
 
vector<int> Bruno(int K, vector<int> value) {
	int tx = 0, ty = 0;
	for (int i = 0;i < 9;i++) {
		if (value[i] == 12) {
			tx = i / 3, ty = i % 3;
		}
	}
	for (int i = 0;i < 9;i++) {
		int id = (i / 3 - tx + 3) % 3 * 3 + (i % 3 - ty + 3) % 3;
		inp[(i / 3 - tx + 3) % 3][(i % 3 - ty + 3) % 3] = value[i];
		px[id] = i / 3;
		py[id] = i % 3;
	}
	int emp = inp[2][2];
	vector<int> ret;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			int num = i * 3 + j;
			if (num > 0 && num <= 7) {
				if (inp[i][j] >= emp) inp[i][j]++;
				if (inp[i][j] >= 9) {
					ret.push_back(inp[i][j] - 9);
				} else {
					int dx = px[num] - 1 + (inp[i][j] - 1) / 3 - 1, dy = py[num] - 1 + (inp[i][j] - 1) % 3 - 1;
					ret.push_back(getans(dx, dy));
				}
			}
		}
	}
	return ret;
}
/*
1
7 7
1 2
1 3
3 2
3 5
4 1
5 3
5 5
4 2
 */
