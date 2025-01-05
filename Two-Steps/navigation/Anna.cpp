#include "Anna.h"
#include <vector>
#include <iostream>
#include <assert.h>
using namespace std;
namespace {
	inline int ab(int x) {
		return x > 0 ? x : -x;
	}
	int getdir(int dx, int dy) {
		int ret = 0;
		if (dy > 1) ret = 0;
		else if (dy < -1) ret = 1;
		else if (dx > 1) ret = 2;
		else ret = 3;
		ret += 9;
		return ret;
	}
	inline int encode(int dx, int dy) {
		return (dx + 1) * 3 + (dy + 1) + 1;
	}
	bool check(int N, int K, vector<int> R, vector<int> C, int mv, bool set) {
		int mx = mv / 3, my = mv % 3;
		int ret[N][N];
		bool found[10];
		for (int i = 0;i < 10;i++) found[i] = false;
		for (int i = 0;i < N;i++) {
			for (int j = 0;j < N;j++) {
				int val = ((i - mx + 3) % 3 * 3 + (j - my + 3) % 3);
				if (val > 0 && val <= 7) {
					int dx = R[val-1] - i, dy = C[val-1] - j;
					if (ab(dx) > 1 || ab(dy) > 1) {
						ret[i][j] = getdir(dx, dy);
					} else {
						ret[i][j] = encode(dx, dy);
						if (encode(dx, dy) == 9) return false;
						found[encode(dx, dy)] = true;
					}
				} else if (val == 0) {
					ret[i][j] = 13;
				} else {
					ret[i][j] = 0;
				}
			}
		}
		int emp = 0;
		for (int i = 1;i < 9;i++) {
			if (!found[i]) {
				emp = i;
				break;
			}
		}
		for (int i = 0;i < N;i++) {
			for (int j = 0;j < N;j++) {
				if (ret[i][j] == 0) ret[i][j] = emp;
				else if (ret[i][j] > emp) {
					ret[i][j]--;
				}
				if (set) {
					SetFlag(i, j, ret[i][j]);
				}
			}
		}
		return true;
	}
} // namespace
 
void Anna(int N, int K, vector<int> R, vector<int> C) {
	for (int i = 0;i < 9;i++) {
		if (check(N, K, R, C, i, false)) {
			check(N, K, R, C, i, true);
			break;
		}
	}
}
