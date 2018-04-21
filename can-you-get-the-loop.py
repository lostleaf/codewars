def loop_size(node):
    p1, p2, steps1, steps2 = node, node, 0, 0
    while True:
        p2 = p2.next.next
        p1 = p1.next
        if p1 == p2:
            break
    while True:
        p2 = p2.next.next
        p1 = p1.next
        steps1 += 1
        steps2 += 2
        if p1 == p2:
            return steps2 - steps1  
