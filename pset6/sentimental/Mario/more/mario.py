from cs50 import *

height = get_int('Height: ')
while height < 1 or height > 23:
    height = get_int('Height: ')

layer = ""

for i in range(height):
    for j in range(height - 1 - i):
        layer += " "
    for j in range(i + 1):
        layer += "#"
    layer += "  "
    for j in range(i + 1):
        layer += "#"

    print(layer)
    layer = ""
