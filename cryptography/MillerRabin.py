###########################################################################
# Created by Iosif. Be pleased to contribute to this shitty python code.
###########################################################################

# Seminar 6
# Miller–Rabin primality test

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

a = 243
n = 1825
t = 0
ans = []
while (i := (n - 1) / 2**t).is_integer():
    ans.append(fast_power(a, int(i), n))
    t += 1
print(ans)
# Criterias are: sequence includes -1 mod n, but not at last position, and first element is 1
if ans[0] == 1 and n - 1 in ans and ans[-1] != -1:
    print('OK')
else:
    print('not OK')
