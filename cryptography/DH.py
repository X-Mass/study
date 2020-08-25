###########################################################################
# Created by Iosif. Be pleased to contribute to this shitty python code.
###########################################################################

# Seminar 7
# Diffie–Hellman

g = 10 # generator
p = 17 # modulo
A = 9 # Alice's public key
B = 13 # Bob's public key

a = 0
b = 0
while (i := (g ** a) % p) != A:
    print('{}^{} mod {} = {}'.format(g, a, p, i))
    a += 1
print('Alice\'s private key: {}'.format(a))
while (i := (g ** b) % p) != B:
    print('{}^{} mod {} = {}'.format(g, b, p, i))
    b += 1
print('Bob\'s private key (if needed): {}'.format(b))
K = (B ** a) % p
print('session key: {}'.format(K))
