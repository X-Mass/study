###########################################################################
# Created by Iosif. Be pleased to contribute to this shitty python code.
###########################################################################

# Seminar 7
# RSA

def inverse_element(a, m): # element, modulo
    if a == 1:
        return 1
    r0 = m
    r1 = a
    y0 = 0
    y1 = 1
    q0 = r0 // r1
    r2 = r0 % r1
    print('{} = {} * {} + {}; y[-2] = {}, y[-1] = {}'.format(r0, q0, r1, r2, y0, y1))
    i = 0
    while r1 != 1:
        r0 = r1
        r1 = r2
        q1 = r0 // r1
        r2 = r0 % r1
        y2 = y0 - y1 * q0
        print('{} = {} * {} + {}; y[{}] = {} - {} * {} = {}'.format(r0, q1, r1, r2, i, y0, y1, q0, y2))
        q0 = q1
        y0 = y1
        y1 = y2
        i += 1
    if y2 < 0:
        y2 += m
    print('inversed to {} element in mod {} is {}'.format(a, m , y2 if y2 > 0 else y2 + m))
    return y2

def fast_power(a, b, m):
    old_ans = ans = 1
    b_binary = "{0:b}".format(b)
    print('power in binary: {}'.format(b_binary))
    n = len(b_binary)
    string = ''
    for i in range(n):
        old_ans = ans
        if b_binary[i] == '1':
            ans = (ans ** 2) * a % m
        else:
            ans = ans ** 2 % m
        string += '({}{})^2 = {}; '.format(old_ans, ' * {}'.format(a) if b_binary[i] == '1' else '', ans)
    print(string)
    return ans

p = 6
q = 11
n = p * q
print('n = {} * {} = {}'.format(p, q, n))
fi = (p - 1) * (q - 1)
print('fi = ({} - 1) * ({} - 1) = {}'.format(p, q, fi))
e = 33
d = inverse_element(e, fi)
print('d = {}^-1 mod {} = {}'.format(e, fi, d))
m = 14 # plain text
c = fast_power(m, e, n) % n
print('c = {}^{} mod {} = {}'.format(m, e, n, c)) # ciper text
M = fast_power(c, d, n) % n
print('m = {}^{} mod {} = {}'.format(c, d, n, M)) # plain text
s = fast_power(m, d, n) % n
print('s = {}^{} mod {} = {}'.format(m, d, n, s)) # sign
M = fast_power(s, e, n) % n
print('m = {}^{} mod {} = {}'.format(s, e, n, M)) # verification
print(s ** e % n)
