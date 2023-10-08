import sys
import re


def main():
    # get card number
    pattern = r"^\d+$"
    while True:
        card_number = input("Number: ")
        if len(card_number) not in [13, 15, 16]:
            print("INVALID")
            sys.exit(1)
        if re.match(pattern, card_number):
            break

    # get card format
    card_format = get_format(card_number)
    if card_format == "":
        print("INVALID")
        sys.exit(1)

    # validate card
    card_valid = validate_card(card_number)
    if not card_valid:
        print("INVALID")
        sys.exit(1)

    print(card_format)

    sys.exit(0)


# Get credit card number format
def get_format(card):
    pattern_amex = r"^(34|37)\d{13}$"
    pattern_visa = r"^4(\d{12}|\d{15})$"
    pattern_mastercard = r"^5[1-5]\d{14}$"

    if re.match(pattern_amex, card):
        return "AMEX"
    elif re.match(pattern_visa, card):
        return "VISA"
    elif re.match(pattern_mastercard, card):
        return "MASTERCARD"

    return ""


# Validate by checksum (Luhn's Algorithm)
def validate_card(card):
    # get sum of every other digit starting from 2nd to last multiplied by 2
    digits_sum = 0
    for digit in card[-2::-2]:
        digit_twice = int(digit) * 2

        if len(str(digit_twice)) > 1:
            for d in str(digit_twice):
                digits_sum += int(d)
        else:
            digits_sum += digit_twice

    # add the remaining numbers
    for digit in card[-1::-2]:
        digits_sum += int(digit)

    if digits_sum % 10 == 0:
        return True
    else:
        return False


main()
