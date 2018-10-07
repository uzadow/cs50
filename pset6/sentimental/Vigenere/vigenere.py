#!/usr/bin/env python3

from cs50 import *

if len(sys.argv) != 2:
    sys.exit("Syntax: python caesar.py [str]")

key = str(sys.argv[1])

if not key.isalpha():
    sys.exit("Syntax: python caesar.py [str]")

key = key.lower()
plain = get_string("plaintext:  ")
cipher = ""
index = 0
key_len = len(key)

for i in plain:
    if i.isupper() and not i == " ":
        cipher += chr(((ord(i) - ord('A') + (ord(key[index % key_len]) - ord('a'))) % 26) + ord('A'))
        index += 1
    elif i.islower() and not i == " ":
        cipher += chr(((ord(i) - ord('a') + (ord(key[index % key_len]) - ord('a'))) % 26) + ord('a'))
        index += 1
    else:
        cipher += i

print("ciphertext: " + cipher)