def solved(s):
    if not isinstance(s, str): return ""
    s, l = list(s), len(s)
    if l % 2: l = (l - 1) // 2; s = s[:l] + s[l+1:]
    return "".join(sorted(s))
