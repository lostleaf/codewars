def brain_luck(code, input):
    st = []
    ret = []
    data = [0]
    op = dict()
    cl = dict()
    for i, c in enumerate(code):
        if c == '[':
            st.append(i)
        if c == ']':
            op[i] = st.pop()
            cl[op[i]] = i
    i = 0
    j = 0
    k = 0
    while i < len(code):
        inst = code[i]
        if inst == '>':
            j += 1
            if j >= len(data):
                data.append(0)
        if inst == '<':
            j -= 1
        if inst == '+':
            data[j] += 1
            if data[j] > 255:
                data[j] = 0
        if inst == '-':
            data[j] -= 1
            if data[j] < 0:
                data[j] = 255
        if inst == '.':
            ret.append(chr(data[j]))
        if inst == ',':
            data[j] = ord(input[k])
            k += 1
        if inst == '[':
            if data[j] == 0:
                i = cl[i]
        if inst == ']':
            if data[j] != 0:
                i = op[i]
        i += 1
    return ''.join(ret)
        
