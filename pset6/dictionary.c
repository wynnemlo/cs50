/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define HASHTABLE_SIZE 65536

// initialize global variables
node* hashtable[HASHTABLE_SIZE];
unsigned int counter = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{   
    // make a lowercase copy of the word
    char word_copy[LENGTH+1];
    
    for (int i = 0, n = strlen(word)+1; i < n; i++)
    {
        // if character is alphabetical, convert to lowercase
        if (isalpha(word[i]))
        {
            word_copy[i] = tolower(word[i]);
        }
        // if character is an apostrophe, just copy
        else
        {
            word_copy[i] = word[i];
        }
    }
    
    // find the bucket it belongs to
    node* current = hashtable[hash_it(word_copy)];
    
    // if list is empty, return false
    if (current == NULL)
    {
        return false;
    }
    // otherwise traverse through the list
    else
    {
        while (current != NULL)
        {
            // if word in current node equals to word given
            if (strcmp(current->word, word_copy) == 0)
            {
                return true;
            }
            else
            {
                current = current->next;
            }
        }
        // if still not found, return false
        return false;
    }
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // declare variables
    char buffer[LENGTH+1];
    int index;
    node* new_node = NULL;
    
    // open file
    FILE* dict = fopen(dictionary, "r");
    
    // check if file opens correctly
    if (dict != NULL)
    {
        // read line-by-line until the end of file
        while (fscanf(dict,"%s\n", buffer) != EOF)
        {
            // find hash value
            index = hash_it(buffer);
            
            // allocate memory for new node
            new_node = malloc(sizeof(node));
            
            // copy string into new node
            strncpy(new_node->word, buffer, LENGTH+1);
            counter++;
            
            // append to beginning of list
            if (hashtable[index] == NULL)
            {
                hashtable[index] = new_node;
                new_node->next = NULL;
            }
            else
            {
                new_node->next = hashtable[index];
                hashtable[index] = new_node;
            }
        }
        // close file
        fclose(dict);
        return true;
    }
    // if file cannot be opened
    else
    {
        return false;
    }
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return counter;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // go through each slot in the array
    for (int i = 0, n = HASHTABLE_SIZE; i < n; i++)
    {
        // if it's not empty, free it
        if (hashtable[i] != NULL)
        {
            node* cursor = hashtable[i];
            while (cursor != NULL)
            {
                node* temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }
    }
    return true;
}

/**
 * Hash function for dictionary.
 * Algorithm & Source code by delipity's husband on CS50 Reddit.
 * <http://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn>
 **/
int hash_it(const char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHTABLE_SIZE;
}
