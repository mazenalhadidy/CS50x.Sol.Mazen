// Implements a dictionary's functionality
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "dictionary.h"
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N * N] = {0};
int wordcount = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int index = hash(word);
    short c = 0;
    for (node *current = table[index]; current != NULL; )
    {
        if (table[index])
        {
            if (strcasecmp(word, current->word) == 0)
            {
                return true;
            }
            //عند عدم وجود خانة مخزنة في الآراي سينتج خطأ تجزئة في
            //current->next
            //لأنه يريد الوصول لخانة ليست محجوزة
            current = current->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int firstletter = (unsigned int)(tolower(word[0]));
    unsigned int secondletter = (unsigned int)(tolower(word[1]));

    //لمعالجة إن كان في القاموس حرف واحد أو أبوستروف
    if (strlen(word) < 2 || secondletter == 39)
    {
        secondletter = 'a';
    }

    unsigned int i1 = (firstletter - 97) * 26;
    unsigned int i2 = secondletter - 97;
    return (i1 + i2);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char wordict[LENGTH + 1];
    int previndex = -1;
    node *prevnode = NULL;

    while (fscanf(dict, "%s", wordict) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, wordict);
        n->next = NULL;

        //اذهب لخانة جديدة في الآراي عند أول كلمة وعند تغير أحد أول حرفين
        if ((wordcount == 0) || (previndex != hash(wordict)))
        {
            table[hash(wordict)] = n;
        }
        else
        {
            prevnode->next = n;
        }

        previndex = hash(wordict);
        prevnode = n;
        wordcount++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < (N * N); i++)
    {
        if (table[i])
        {
            for (node* current = table[i]; current != NULL; )
            {
                node* tmp = current;
                current = current->next;
                if (tmp)
                {
                    free(tmp);
                }
            }
        }
    }

    return true;
}
