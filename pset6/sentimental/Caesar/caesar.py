from cs50 import *

if len(sys.argv) != 2:
    sys.exit("Syntax: python caesar.py [int]")

key = int(sys.argv[1])
plain = get_string("plaintext:  ")
cipher = ""

for i in plain:
    if i >= 'A' and i <= 'Z':
        cipher += chr(((ord(i) - ord('A') + key) % 26) + ord('A'))
    elif i >= 'a' and i <= 'z':
        cipher += chr(((ord(i) - ord('a') + key) % 26) + ord('a'))
    else:
        cipher += i

print("ciphertext: " + cipher)