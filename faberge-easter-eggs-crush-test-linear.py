MOD = 998244353
haha_inv = [0, 1]
for haha_i in range(2, 80000 + 1):
        haha_inv.append( (MOD - MOD // haha_i) * haha_inv[MOD % haha_i] % MOD )
def height(n, m):  
    h, t = 0, 1
    m %= MOD

    for i in range(1, n + 1): 
        t = t * (m - i + 1) * haha_inv[i] % MOD
        h = (h + t) % MOD
    return h % MOD