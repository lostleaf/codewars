#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <iostream>


using namespace std;

const int N = 7;
const int SIDES = 4;
const int MASK = (1 << N) - 1;
const int s[SIDES * N] = {0,1,2,3,4,5,6,6,13,20,27,34,41,48,48,47,46,45,44,43,42,42,35,28,21,14,7,0};
const int inc[SIDES * N] = {7,7,7,7,7,7,7,-1,-1,-1,-1,-1,-1,-1,-7,-7,-7,-7,-7,-7,-7,1,1,1,1,1,1,1};
int possible[N * N], results[N][N];
bool vis[N * N];
vector<int> my_clues;

void set_value(int x, int v) {
    int m = MASK ^ (1 << v);
    int s_row = x - x % N;
    int s_col = x % N;
    for (int i = 0; i < N; i++) {
        possible[s_row + i] &= m;
        possible[s_col + i * N] &= m;
    }
    possible[x] = 1 << v;
}

int check_unique() {
    int n_decides = 0;
    for (int i = 0; i < SIDES / 2 * N; i++) {
        map<int, vector<int> > possible_indices;
        for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
            for (int l = 0; l < N; l++) 
                if ((1 << l) & possible[j]) {
                    possible_indices[l].push_back(j);
                }
        }

        for (auto const &iter : possible_indices) {
            int val = iter.first;
            if (iter.second.size() == 1) {
                int idx = iter.second[0];
                if (possible[idx] != (1 << val)) {
                    n_decides++;
                    set_value(idx, val);
                }
            }
        }
    }
    return n_decides;
}

int filter2 () {
    int cnt = 0;
    for (int i = 0; i < SIDES * N; i++) {
        if (my_clues[i] == 2) {
            int mask = MASK;
            for (int l = N - 1; l >= 0; l--) {
                int m = (1 << l) & possible[s[i]];
                mask ^= 1 << l;
                if (m) break;
            }

            for (int j = s[i] + inc[i], k = 1; k < N; j += inc[i], k++) {
                int m = (1 << (N - 1)) & possible[j];
                if (m) break;
                if ((possible[j] | mask) != mask) {
                    possible[j] &= mask;
                    cnt++;
                }
            }
        }
    }
    return cnt;
}

int count_possible(int val) {
    int n = 0;
    while (val) {
        n += val & 1;
        val >>= 1;
    }
    return n;
}

bool valid() {
    for (int i = 0; i < SIDES * N; i++) {
        if (my_clues[i] == 0) continue;

        bool is_decided = true;
        for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
            if (count_possible(possible[j]) != 1) {
                is_decided = false;
                break;
            }
        }

        if (is_decided) {
            int largest = 0, n_clue = 0;
            for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
                if (largest < possible[j]) {
                    n_clue++;
                    largest = possible[j];
                }
            }
            if (n_clue != my_clues[i]) return false;
        }
    }

    return true;
}

void write_results() {
    for (int i = 0; i < N * N; i++) {
        int x = i / N, y = i % N;
        for (int j = 0; j < N; j++) {
            if ((1 << j) == possible[i]) {
                results[x][y] = j + 1;
                break;
            }
        }
    }
}

bool dfs(int idx) {
    int i = -1, tmp = 10000;
    for (int _i = 0; _i < N * N; _i++) {
        int c = count_possible(possible[_i]);
        if (tmp > c && !vis[_i]) {
            tmp = c;
            i = _i;
        }
    }

    if (i == -1) {
        if (valid()) {
            write_results();
            return true;
        }
        return false;
    }

    int possible_bak[N * N];
    memcpy(possible_bak, possible, sizeof(int) * N * N);

    for (int j = N - 1; j >= 0; j--) {

        int m = (1 << j) & possible[i];
        if (m == 0) continue;

        vis[i] = true;
        set_value(i, j);
        bool found = false;
        if (valid()) {
            found = dfs(idx + 1);
        }
        vis[i] = false;
        memcpy(possible, possible_bak, sizeof(int) * N * N);
        if (found) {
            return true;
        }
    }
    return false;
}

void init() {
    for (int i = 0; i < N * N; i++) {
        possible[i] = MASK;
        vis[i] = true;
    }
}

void pre_process() {
    for (int i = 0; i < SIDES * N; i++) 
    {
        if (my_clues[i] == 0) continue;
        for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
            int m = MASK;
            for (int l = N + k - my_clues[i] + 1; l < N; l++) m ^= 1 << l;
            possible[j] &= m;
        }
    }

    while (check_unique() > 0 );
    filter2();
}

vector<vector<int>> SolvePuzzle(const vector<int> &clues) {
    vector<vector<int>> r;
    init();
    my_clues = clues;

    pre_process();

    vector<pair<int, int>> idx_npos;
    for (int i = 0; i < N * N; i++) {
        int n_possible = count_possible(possible[i]);
        if (n_possible > 1) {
            vis[i] = false;
        }
    }

    dfs(0);

    for (int i = 0; i < N; i++) {
        vector<int> vec;
        for (int j = 0; j < N; j++) vec.push_back(results[i][j]);
        r.push_back(vec);
    }
    return r;
}
