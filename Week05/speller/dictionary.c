// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

unsigned int dict_words = 0;
const unsigned int N = 676; // 26 * 26

// Hash table
node *table[N];

// Hashes word to a number
unsigned int hash(const char *word)
{
    int length = strlen(word);
    if (length >= 26)
    {
        length = 26;
    }
    return (toupper(word[0]) - 'A') * 26 + (length - 1);
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hashed = hash(word);
    node *ptr = table[hashed];

    while (ptr != NULL)
    {
        if (strcasecmp(ptr->word, word) == 0)
        {
            return true;
        }

        ptr = ptr->next;
    }
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    node *new_node = NULL;
    while (true)
    {
        // no word is longer than 45 letters
        char word[LENGTH + 1] = "";
        int scanres = fscanf(dict, "%s", word);
        if (scanres == EOF)
        {
            break;
        }
        else if (scanres == 1)
        {
            new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                printf("Error: Something went wrong.");
                fclose(dict);
                return false;
            }
            int hashed = hash(word);

            for (int i = 0, n = strlen(word); i < n; i++)
            {
                word[i] = tolower(word[i]);
            }

            strcpy(new_node->word, word);
            new_node->next = NULL;

            if (table[hashed] != NULL)
            {
                new_node->next = table[hashed];
            }

            table[hashed] = new_node;
            dict_words++;
        }
        else
        {
            printf("Error reading from file.\n");
            fclose(dict);
            return false;
        }
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *temp = ptr;
            ptr = ptr->next;
            free(temp);
        }
    }

    return true;
}
