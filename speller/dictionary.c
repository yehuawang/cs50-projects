// Implements a dictionary's functionality
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

// global variables
unsigned int dict_size = 0; // the size of the dictionary.

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char lower_word[strlen(word) + 1];
    for (int i = 0; i < strlen(word); i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    lower_word[strlen(word)] = '\0';

    unsigned int hashed_index = hash(lower_word);
    node *ptr = table[hashed_index];

    while (ptr != NULL)
    {
        if (strcmp((ptr->word), lower_word) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    const char *ptr = word;
    unsigned int k = 0;
    const double A = 0.618033988749;
    for (int i = 0; *(ptr + i) != '\0'; i++)
    {
        k += *(ptr + i) - 'a';
    }
    unsigned int hashed = (unsigned int) floor(N * fmod((k * A), 1.0));
    return hashed;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // initialized all buckets to NULL
    for (int i = 0; i < N; i++)
    {
        *(table + i) = NULL;
    }

    // read file dictionaries
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        // failed to read dictionary
        return false;
    }

    // create buffer
    char buffer[LENGTH + 1] = {'\0'};

    // using fgets to get line by line:
    while (fgets(buffer, LENGTH + 2, dict) != NULL)
    {
        // remove '\n'
        buffer[strcspn(buffer, "\n")] = '\0';
        // hash the buffer
        unsigned int hashed_index = hash(buffer);
        // create the node to insert:
        node *word_node = malloc(sizeof(node));
        if (word_node == NULL)
        {
            fclose(dict);
            return false;
        }
        strcpy(word_node->word, buffer);
        word_node->next = NULL;

        // if bucket is empty, insert here
        if (table[hashed_index] == NULL)
        {
            table[hashed_index] = word_node;
        }
        // else if bucket is not empty, go to next in linked list until empty
        else
        {
            node *ptr = table[hashed_index];
            while (ptr->next != NULL)
            {
                ptr = ptr->next;
            }
            // found empty spot, insert value at ptr->next
            ptr->next = word_node;
        }
        dict_size++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *tofree = ptr;
            ptr = ptr->next;
            free(tofree);
        }
        table[i] = NULL;
    }
    return true;
}
