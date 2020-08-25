###########################################################################
# Created by Iosif. Be pleased to contribute to this shitty python code.
###########################################################################

# Seminar 8
# Elgamal

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

p = 13
g = 11
# If we have to find y from x
x = 9
# y = fast_power(g, x, p)
# If we have to find y from x, do x = 0
y = 8
# x = 0
# while (i := (g ** x) % p) != y:
#     print('{}^{} mod {} = {}'.format(g, x, p, i))
#     x += 1
# print('y = {}^{} = {} mod {}'.format(g, x, y, p))
m = 3 # plain text
k = 5
a = fast_power(g, k, p)
print('a = {}^{} = {} mod {}'.format(g, k, a, p))
b = (fast_power(y, k, p) * m) % p
print('b = {}^{} * {} = {} mod {}'.format(y, k, m, b, p))
print('encrypted: ({}, {})'.format(a, b))
# # If we have to decrypt
# a = 11
# b = 11
k1 = fast_power(a, x, p)
k2 = inverse_element(k1, p)
M = (b * k2) % p
print('decrypted M = {} / {} = {} * {} = {} mod {}'.format(b, k1, b, k2, M, p)) # decrypted text
# If we have to sign
M = 6
b = (M - x * a) * inverse_element(k, p - 1)
while b < 0:
    b += p - 1
print('b = ({} - {} * {}) * {}^-1 = {} mod {}'.format(M, x, a, k, b, p - 1))
print('sign: ({},{})'.format(a, b))
print('sign check {}^{} * {}^{} == {}^{} mod {}; {} == {} mod {}'.format(y, a, a, b, g, M, p, fast_power(y, a, p) * fast_power(a, b, p) % p, fast_power(g, M, p), p))
