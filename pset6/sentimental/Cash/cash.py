from cs50 import *
import math

change = get_float("Change owed: ")
while change <= 0:
    change = get_float("Change owed: ")

change = round(change * 100)
print(math.floor(change / 25) + math.floor(change % 25 / 10) + math.floor(change % 25 % 10 / 5) + math.floor(change % 25 % 10 % 5))