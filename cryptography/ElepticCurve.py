###########################################################################
# Created by Iosif. Be pleased to contribute to this shitty python code.
###########################################################################

# Seminar 9
# Eleptic curve

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

# EC points addition
def add_point_to_point(x1, y1, x2, y2, mod, λ = -1):
    if x1 == x2 and y1 != y2:
        print('{} == {} and {} != {} => "0"'.format(x1, x2, y1, y2))
        return -1, -1
    if λ == -1:
        print('λ = ({} - {}) / ({} - {}) = {} / {} = {} * {} = {} mod {}'.format(y1, y2, x1, x2, k1 := (y1 - y2) % mod, k2 := (x1 - x2) % mod, k1, inv := inverse_element(k2, mod), λ := (inv * k1) % mod, mod))
    x = (λ**2 - x1 - x2) % mod
    print('x = {}^2 - {} - {} = {} mod {}'.format(λ, x1, x2, x, mod))
    y = (-y2 + λ * (x2 - x)) % mod
    print('y = -{} + {}({} - {}) = {} mod {}'.format(y2, λ, x2, x, y, mod))
    return x, y

# Draw table for Edvard's curve
# y^2 = x^3 + ax^2 + b
a = -3
b = -13
mod = 17
# All points table
# x2 = []
# x3 = []
# right = []
# print('x, x^2, x^3, x^3 - {}x + {}'.format(a, b))
# for x in range(mod):
#     x2.append(x**2 % mod)
#     x3.append(x**3 % mod)
#     right.append((x**3 + a * x + b) % mod)
#     print(x, x2[x], x3[x], right[x])
# for i in range(mod):
#     indices = [j for j, x in enumerate(x2) if x == right[i]]
#     for j in indices:
#         print('({}, {})'.format(i, j))
# print('AND "0"!')
# Generator or sign stuff
xn, yn = x, y = 15, 6 # also needed for sign (G)
# λ is also needed for sign
λ = 0
print('λ = (3 * {}^2 + {}) / (2 * {}) = {} / {} = {} * {} = {} mod {}'.format(x, a, y, k1 := (3 * x**2 + a) % mod, k2 := (2 * y) % mod, k1, inv := inverse_element(k2, mod), λ := (inv * k1) % mod, mod))
# # Check if point is a generator
# print('group or subgroup is:')
# # first iteration λ differs, so made it stay outside
# print('({},{})'.format(xn, yn))
# xn, yn = add_point_to_point(x, y, xn, yn, mod, λ)
# while xn != -1:
#     print('({},{})'.format(xn, yn))
#     xn, yn = add_point_to_point(x, y, xn, yn, mod)
# print('AND "0"!')
# sign operation GOST 34
m = 5 # plain text
# Use (x, y) instead of (Gx, Gy)
Qx, Qy = 8, 4
k = 4
n = 15 # make equal to mod if not given
# Q can be equal to (x, y), but it's kinda stupid to make such tasks
d = 2
xn, yn = x, y
print('({},{})'.format(xn, yn))
x_old = [xn]
xn, yn = add_point_to_point(x, y, xn, yn, mod, λ)
x_old.append(xn)
while xn != Qx and yn != Qy:
    d += 1
    xn, yn = add_point_to_point(x, y, xn, yn, mod)
    x_old.append(xn)
print('d = {}'.format(d))
if k < d:
    xn = x_old[k - 1]
for _ in range (k - d):
    xn, yn = add_point_to_point(x, y, xn, yn, mod)
s = (xn * d + k * m) % n
print('s = {} * {} + {} * {} = {} mod {}'.format(xn, d, k, m, s, n))
print('sign: ({}, {})'.format(xn, s))
