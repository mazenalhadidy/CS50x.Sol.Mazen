# TODO
from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height <= 8 and height >= 1:
        break

for space in range(1, height + 1):
    for count in reversed(range(height - space)):
        print(" ", end="")
    for count in range(space):
        print("#", end="")
    print("  ", end="")
    for count in range(space):
        print("#", end="")
    print()
