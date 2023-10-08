from cs50 import get_int


def main():
    height = get_height()
    for i in range(1, height + 1):
        print(" " * (height - i) + "#" * i, end="  ")
        print("#" * i)


def get_height():
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            return height


main()
