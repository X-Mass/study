###########################################################################
# Created by Iosif. Be pleased to contribute to this shitty python code.
###########################################################################

# Seminar 10
# Diffie–Hellman on eleptic curves

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

# y^2 = x^3 + ka * x^2 + kb
Gx, Gy = 15, 3
ka = -3
kb = -6
mod = 17
a = 2 # Alice's private key
b = 3 # Bob's private key
λ = 0
print('λ = (3 * {}^2 + {}) / (2 * {}) = {} / {} = {} * {} = {} mod {}'.format(Gx, ka, Gy, k1 := (3 * Gx**2 + ka) % mod, k2 := (2 * Gy) % mod, k1, inv := inverse_element(k2, mod), λ := (inv * k1) % mod, mod))
λ_old = λ
Ax, Ay = Gx, Gy
# a can be 1, but it's kinda stupid to make such tasks
Ax, Ay = add_point_to_point(Ax, Ay, Gx, Gy, mod, λ)
for _ in range (a - 2):
    Ax, Ay = add_point_to_point(Ax, Ay, Gx, Gy, mod)
print('Alice\'s public key: ({}, {})'.format(Ax, Ay))
print('λ = (3 * {}^2 + {}) / (2 * {}) = {} / {} = {} * {} = {} mod {}'.format(Ax, ka, Ay, k1 := (3 * Ax**2 + ka) % mod, k2 := (2 * Ay) % mod, k1, inv := inverse_element(k2, mod), λ := (inv * k1) % mod, mod))
Sx, Sy = Ax, Ay
Sx, Sy = add_point_to_point(Sx, Sy, Ax, Ay, mod, λ)
for _ in range (b - 2):
    Sx, Sy = add_point_to_point(Sx, Sy, Ax, Ay, mod)
print('Session key from Alice\'s public key: ({}, {})'.format(Sx, Sy))
# Now the same for Bob
Bx, By = Gx, Gy
# a can be 1, but it's kinda stupid to make such tasks
Bx, By = add_point_to_point(Bx, By, Gx, Gy, mod, λ_old)
for _ in range (b - 2):
    Bx, By = add_point_to_point(Bx, By, Gx, Gy, mod)
print('Bob\'s public key: ({}, {})'.format(Bx, By))
print('λ = (3 * {}^2 + {}) / (2 * {}) = {} / {} = {} * {} = {} mod {}'.format(Bx, ka, By, k1 := (3 * Bx**2 + ka) % mod, k2 := (2 * By) % mod, k1, inv := inverse_element(k2, mod), λ := (inv * k1) % mod, mod))
Sx, Sy = Bx, By
Sx, Sy = add_point_to_point(Sx, Sy, Bx, By, mod, λ)
for _ in range (a - 2):
    Sx, Sy = add_point_to_point(Sx, Sy, Bx, By, mod)
print('Session key from Bob\'s public key: ({}, {})'.format(Sx, Sy))
