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
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];
int words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    int hashNumber = hash(word);

    node *pointer = table[hashNumber];

    for (node *tempNode = pointer; tempNode != NULL; tempNode = tempNode->next)
    {
        if (strcasecmp(tempNode->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int wordLength = strlen(word);
    char firstChar = toupper(word[0]);
    int firstValue = (int)firstChar;
    int hashedValue = 0;

    if (wordLength > 1)
    {
        char secondChar = toupper(word[1]);
        int secondValue = (int)secondChar;

        for (int i = 0; i < 25; i++)
        {
            if ((firstValue - 65) == i)
            {
                secondValue = secondValue + (25 * i);
            }
        }

        int totalValue = firstValue + secondValue;
        hashedValue = totalValue - 130;
    }
    else
    {
        hashedValue = firstValue - 65;
    }

    return hashedValue;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Opens file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *tempNode = malloc(sizeof(node));
        if (tempNode == NULL)
        {
            unload();
            return false;
        }

        int n = hash(word);

        strcpy(tempNode->word, word);

        tempNode->next = table[n];
        table[n] = tempNode;
        words++;
    }
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *pointer = table[i];

        while (pointer != NULL)
        {
            node *tempNode = pointer;
            pointer = pointer->next;
            free(tempNode);
        }

        free(pointer);
    }

    return true;
}
