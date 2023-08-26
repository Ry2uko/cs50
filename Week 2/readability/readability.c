#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_text(string text, char type);
int get_index(int letters, int words, int sentences);

int main(void)
{
    // init
    string text = get_string("Text: ");
    int letters = count_text(text, 'l');
    int words = count_text(text, 'w');
    int sentences = count_text(text, 's');

    // output grade level
    int grade_level = get_index(letters, words, sentences);

    if (grade_level < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade_level >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade_level);
    }

    return 0;
}

// Count the letters (l), sentences (s), or words (w) in the given text
int count_text(string text, char type)
{
    int letters = 0;
    int words = 1;
    int sentences = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char upper = toupper(text[i]);

        // letters: A~Z
        if (upper >= 65 && upper <= 90)
        {
            letters += 1;
        }

        // words: SP
        if (text[i] == 32)
        {
            words += 1;
        }

        // sentences: !, ., ?
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            sentences += 1;
        }
    }

    if (type == 'l')
    {
        return letters;
    }
    else if (type == 'w')
    {
        return words;
    }
    else
    {
        return sentences;
    }
}

// Get grade level by Coleman-Liau index
// index = 0.0588 * L - 0.296 * S - 15.8
int get_index(int letters, int words, int sentences)
{
    // calculate L
    double l = ((float) letters / words) * 100;

    // calculate S
    double s = ((float) sentences / words) * 100;

    double index = (0.0588 * l) - (0.296 * s) - 15.8;

    return round(index);
}