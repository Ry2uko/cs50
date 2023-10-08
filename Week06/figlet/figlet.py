from pyfiglet import Figlet
import sys
import random


def main():
    figlet = Figlet()
    figlet_fonts = figlet.getFonts()

    if len(sys.argv) == 1:
        rand_int = random.randint(0, len(figlet_fonts) - 1)

        figlet.setFont(font=figlet_fonts[rand_int])
    elif len(sys.argv) == 3:
        if sys.argv[1] not in ["-f", "--font"]:
            exit(1)
        if sys.argv[2].lower() not in figlet_fonts:
            exit(1)

        figlet.setFont(font=sys.argv[2].lower())
    else:
        exit(1)

    text = input("Input: ")
    print(figlet.renderText(text))
    exit(0)


def exit(s):
    if s:
        print("Invalid usage")
        sys.exit(1)
    else:
        sys.exit(0)


main()
