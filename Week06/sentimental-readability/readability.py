import sys
import math


def main():
    # get text
    text = input("Text: ")

    # count text
    text_dict = count_text(text)

    # calculate grade level
    index = calculate_index(text_dict)

    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")

    sys.exit(0)


# count words, letters, and sentences
def count_text(text):
    text_dict = {
        "letters": 0,
        "words": 1,  # last word
        "sentences": 0,
    }

    for c in text:
        if c.isalpha():
            text_dict["letters"] += 1

        if c == " ":
            text_dict["words"] += 1

        if c in ["!", ".", "?"]:
            text_dict["sentences"] += 1

    return text_dict


# calculate grade level by Coleman-Liau index
# index = 0.0588 * L - 0.296 * S - 15.8
# where, L = average number of letters per 100 words
# S = average number of sentences per 100 words
def calculate_index(text_dict):
    letters, words, sentences = (
        text_dict["letters"],
        text_dict["words"],
        text_dict["sentences"],
    )
    l = (letters / words) * 100
    s = (sentences / words) * 100
    index = (0.0588 * l) - (0.296 * s) - 15.8

    return round(index)


main()
