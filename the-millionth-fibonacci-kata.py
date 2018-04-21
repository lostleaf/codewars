def mul(mat1, mat2):
    mat3 = []
    for i in xrange(2):
        r = []
        for j in xrange(2):
            t = 0
            for k in xrange(2):
                t += mat1[i][k] * mat2[k][j]
            r.append(t)
        mat3.append(r)
    return mat3

def fib(n):
    r = [[1, 0], [0, 1]]
    t = [[0, 1], [1, 1]]
    if n < 0:
        n = -n
        t = [[-1, 1], [1, 0]]
    while n > 0:
        if (n & 1) > 0:
            r = mul(t, r)
        t = mul(t, t)
        n >>= 1
    return r[0][1]
