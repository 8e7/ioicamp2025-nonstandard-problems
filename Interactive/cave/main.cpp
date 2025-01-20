#include "lib1839.h"
#define maxn 5005
#define ll long long
using namespace std;
int cur[maxn], done[maxn], d[maxn];
int siz = 0;
int main() {
	while (true) {
		siz = Initialize();
		for (int i = 0;i < siz;i++) d[i] = cur[i] = done[i] = 0;
		for (int i = 0;i < siz;i++) {
			int low = 0, up = siz, mid;
			int len = tryCombination(cur);
			if (len == -1) len = siz;
			int con = len > i ? 1 : 0; //the switch state which closes door i
			while (low < up - 1) {
				mid = (low + up) / 2;
				for (int j = low;j < mid;j++) {
					if (!done[j]) cur[j] ^= 1;
				}
				len = tryCombination(cur);
				if (len == -1) len = siz;
				if ((len > i ? 1 : 0) != con) {
					up = mid;
				} else {
					low = mid;
				}
				for (int j = low;j < mid;j++) {
					if (!done[j]) cur[j] ^= 1;
				}
			}
			done[low] = 1;
			d[low] = i;
			if (!con) cur[low] ^= 1;
		}
		answer(cur, d);
	}
}
