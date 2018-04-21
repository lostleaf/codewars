#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

const int N = 6;
const int SIDES = 4;
const int MASK = (1 << N) - 1;
int possible[N * N], s[SIDES * N], e[SIDES * N], inc[SIDES * N];
int results[N][N];
vector<int> my_clues;
vector<int> order;

void print_binary(int x) {
    for (int i = N - 1; i >= 0; i--) printf("%d", (x & (1 << i)) ? 1 : 0);
}

void print_possible() {
    for (int i = 0; i < N * N; i++) {
        print_binary(possible[i]);
        printf(" ");
        if (i % N == N - 1) printf("\n");
    }
}

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
    // printf("%d %d\n", idx, order[idx]);
    if (idx >= order.size()) {
        if (valid()) {
            write_results();
            return true;
        }
        return false;
    }

    int i = order[idx];
    int possible_bak[N * N];
    memcpy(possible_bak, possible, sizeof(int) * N * N);

    for (int j = 0; j < N; j++) {
        int m = (1 << j) & possible[i];
        if (m == 0) continue;

        set_value(i, j);
        bool found = false;
        if (valid()) {
            found = dfs(idx + 1);
        }
        if (found) {
            return true;
        }
        memcpy(possible, possible_bak, sizeof(int) * N * N);
    }
    return false;
}

vector<vector<int>> SolvePuzzle(const vector<int> &clues) {
    my_clues = clues;
    for (int i = 0; i < N * N; i++) possible[i] = MASK;

    for (int i = 0; i < N; i++) {
        s[i] = i;
        e[i] = (N - 1) * N + i;
        inc[i] = N;
    }

    for (int i = 0, j = N; i < N; i++, j++) {
        s[j] = i * N + N - 1;
        e[j] = i * N;
        inc[j] = -1;
    }

    for (int i = 0, j = 2 * N; i < N; i++, j++) {
        s[j] = N * N - 1 - i;
        e[j] = N - 1 - i;
        inc[j] = -N;
    }

    for (int i = 0, j = 3 * N; i < N; i++, j++) {
        s[j] = (N - i - 1) * N;
        e[j] = (N - i) * N - 1;
        inc[j] = 1;
    }

    for (int i = 0; i < SIDES * N; i++) 
    {
        // int i = 12;
        if (my_clues[i] == 0) continue;
        for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
            int m = MASK;
            for (int l = N + k - my_clues[i] + 1; l < N; l++) m ^= 1 << l;
            possible[j] &= m;
        }
        // printf("%d %d\n", i, my_clues[i]);
        // print_possible();
    }

    while (check_unique() > 0);

    vector<pair<int, int>> idx_npos;
    for (int i = 0; i < N * N; i++) {
        int n_possible = count_possible(possible[i]);
        if (n_possible > 1) {
            idx_npos.push_back(make_pair(n_possible, i));
        }
    }

    sort(idx_npos.begin(), idx_npos.end());
    order.clear();
    for (int i = 0; i < idx_npos.size(); i++) {
        order.push_back(idx_npos[i].second);
    }
    dfs(0);

    vector<vector<int>> r;
    for (int i = 0; i < N; i++) {
        vector<int> vec;
        for (int j = 0; j < N; j++) vec.push_back(results[i][j]);
        r.push_back(vec);
    }
    return r;
}
