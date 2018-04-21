def partitions(n):
    '''define a function which returns the number of integer partitions of n'''
    f = [[1] * (n + 1)]
    for i in xrange(1, n + 1):
        f.append([0])
        for j in xrange(1, i + 1):
            t = f[i - j][i - j] if j > i - j else f[i - j][j]
            f[i].append(f[i][j - 1] + t)
    return f[n][n]
