#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <time.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000

typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list;
    int occurrences1; //how many times it shows in the text
    int length; //length of the WordProbability
} WordStruct;

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    int occurrences2; //how many times it shows at probability list after word
} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add(LinkList *link_list, WordStruct *data)
{
    Node *new_node = (Node*)(malloc(sizeof(Node)));
    if (new_node == NULL)
    {
        return 1;
    }
    *new_node = (Node){data, NULL};

    if (link_list->first == NULL)
    {
        link_list->first = new_node;
        link_list->last = new_node;
    }
    else
    {
        link_list->last->next = new_node;
        link_list->last = new_node;
    }

    link_list->size++;
    return 0;
}
/*************************************/

/**
 * Get .
 * @param *ptr
 * @return EXIT_FAILURE, exits with error from program
 */
void checkFailed(void* ptr)
{
    if(ptr == NULL) //Check if the assignment was successful
    {
        printf("Allocation failure: malloc failed\n"); //Comes out with an error message
        exit(EXIT_FAILURE); //And finishes
    }
}

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand()%max_number; //Get random number between 0 and max_number (0, max_number)
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word(LinkList *dictionary)
{
    Node *temp = dictionary->first; //Pointer to the top of the list
    int num = get_random_number(dictionary->size); //We will send the dictionary size to the method that raffle number

    while (1) //infinite loop
    {
        while (temp != NULL && num > 0) //As long as you are not equal to NULL and the number is not equal to 0
        {
            num--; //We will reduce the number
            temp = temp->next; //Let's move on to the next limb
        }

        if(temp->data->prob_list != NULL) //word without '.'
        {
            return temp->data; //Bring her back
        }
        else //If it does end in '.'
        {
            num = get_random_number(dictionary->size); //We will send the dictionary size to the method that raffle number again
            temp = dictionary->first; //We will initialize Temp to be the top of the list again
        }
    }
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word(WordStruct *word_struct_ptr)
{
    int i;
    int randomNum = get_random_number(word_struct_ptr->occurrences1); //We will send the occurrences1 of the wordstruct to the method that raffle number
    for(i = 0 ; i < word_struct_ptr->length; i++) //We will run in a loop on the size of the array
    {
        if(word_struct_ptr->prob_list[i].occurrences2 >= randomNum) //We will check whether the number of occurrences of the word Large / equal to the number drawn
        {
            return word_struct_ptr->prob_list[i].word_struct_ptr; //If so, we will choose this word as the next word - we will return it
        }
        else //if not
        {
            randomNum-=word_struct_ptr->prob_list[i].occurrences2; //We will reduce the random number in the number of occurrences of the word tested and continue to test the next word
        }
    }
    return word_struct_ptr->prob_list[i].word_struct_ptr; //we will choose this word as the next word - we will return it
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence(LinkList *dictionary)
{
    WordStruct *first, *next;
    first = get_first_random_word(dictionary); //We will perform a raffle for the first word of the tweet
    printf("%s ", first->word);
    int lenOfSen = 1; //The length of the tweet is currently 1
    int stop = 0;

    while (stop != 1 && lenOfSen != MAX_WORDS_IN_SENTENCE_GENERATION) //Until we get word with '.' or up to a length of 20
    {
        next = get_next_random_word(first); //Sent to a method that will draw a lottery for a second word
        printf("%s ", next->word);
        lenOfSen+=1; //We will increase the number of words by 1

        if(next->prob_list == NULL) //finish with '.'
        {
            stop = 1; //We will stop the loop because we finished at the point
        }
        first = next; //Let's move on next
    }
    return lenOfSen; //return Amount of words in printed sentence
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list(WordStruct *first_word, WordStruct *second_word)
{
    char *str = (char*)(malloc(sizeof(char *) * strlen(first_word->word) + 1)); //Assignment the size of the first word
    checkFailed(str); //Check if the assignment was successful
    strcpy(str, first_word->word); //We will copy the first word into it

    if(str[strlen(str)-1] == '.') //Check if the word ends in '.'
    {
        free(str); //Release the pointer
        return 1;
    }
    free(str); //Release the pointer

    if(first_word->prob_list == NULL) //array is empty
    {
        first_word->prob_list = (WordProbability*)(malloc(sizeof(WordProbability))); //Allocation of the array
        checkFailed(first_word->prob_list); //Check if the assignment was successful
        first_word->prob_list[0].word_struct_ptr = second_word; //The first place in the array will indicate the second word
        first_word->prob_list[0].occurrences2 = 1; //The number of performances will be 1
        first_word->length = 1; //The length of the array is size 1
        return 1;
    }

    int i;
    for(i = 0; i < first_word->length; i++) //We will run on the whole array
    {
        if(strcmp(first_word->prob_list[i].word_struct_ptr->word, second_word->word) == 0) //If the word already exists in the array
        {
            first_word->prob_list[i].occurrences2+=1; //We will increase the number of shows
            return 0;
        }
    }
    first_word->prob_list = (WordProbability*)realloc(first_word->prob_list,(first_word->length+1)*sizeof(WordStruct)); //If it does not exist in the array we will have to assign it one more space
    checkFailed(first_word->prob_list); //Check if the assignment was successful
    first_word->prob_list[first_word->length].word_struct_ptr = second_word; //The last place in the array will indicate the second word
    first_word->prob_list[first_word->length].occurrences2 = 1; //The number of performances will be 1
    first_word->length++; //We will increase the length of the array by 1
    return 1;
}

/**
 * This method check if the word exists in the list.
 * update the occurrences1.
 * Otherwise, Allocates space and updates all other parameters.
 * @param word to check if it exists
 * @param dictionary Dictionary to run of it
 * @return WordStruct of the word
 */
WordStruct* fill_help(char *word, LinkList *dictionary)
{
    WordStruct *data;
    Node *temp = dictionary->first; //Pointer to the top of the list

    while (temp != NULL) //As long as we did not reach the end of the list
    {
        if(strcmp(temp->data->word, word) == 0) //We will check if the word already exists in the dictionary
        {
            temp->data->occurrences1+=1; //We will increase the number of instances
            free(word); //We will release the pointer we have assigned to the word
            return temp->data; //We will return the Wordstruct
        }
        else
        {
            temp = temp->next; //We will move on in the list next
        }
    }
    data = (WordStruct*)(malloc(sizeof(WordStruct))); //If the word does not exist, we will have to assign it
    checkFailed(data); //Check if the assignment was successful
    data->length = 0; //Boot length at 0
    data->prob_list = NULL; //Initialize the array at NULL
    data->word = word; //Will point to the new word
    add(dictionary, data); //Will be sent to the method that adds to the linked list
    dictionary->last->data->occurrences1 = 1; //The number of performances will be 1
    return dictionary->last->data; //We will return a pointer to the word
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary(FILE *fp, int words_to_read, LinkList *dictionary)
{
    int stop;
    if(words_to_read == 0) //read all the text
    {
        stop = -1;
    }
    else
    {
        stop = words_to_read; //We will enter the number of words that need to be read
    }

    char *token, *token1, *token2;
    WordStruct *temp, *temp1;
    char buffer[MAX_SENTENCE_LENGTH];

    while(fgets(buffer, MAX_SENTENCE_LENGTH, fp) != NULL) //We will read the whole file
    {
        token = strtok(buffer, " \n"); //Into the token pointer enters the first word of the string str until the space
        if(token == NULL)  //If the word read points to NULL we will stop
        {
            break;
        }
        token1 = (char*)malloc(sizeof(char)*strlen(token)+1); //We will assign the size of the word we read
        checkFailed(token1); //Check if the assignment was successful
        strcpy(token1, token); //We will copy the word
        stop--; //We will subtract from the number of words we read
        temp = fill_help(token1, dictionary); //We will send to the auxiliary method the word we read that will add it to the dictionary
        while (token != NULL) //As long as you're not done cutting the sentence and you're not equal to NULL
        {
            if(stop == 0) //We finished reading the number of words we were asked
            {
                return; //will finish
            }
            token = strtok(NULL, " \n"); //Cut the words one by one according to the space between them
            if(token == NULL) //If the word read points to NULL we will stop
            {
                break;
            }
            token2 = (char*)malloc(sizeof(char) * strlen(token) + 1); //We will assign the size of the word we read
            checkFailed(token2); //Check if the assignment was successful
            strcpy(token2, token); //We will copy the word                 ;
            stop--; //We will subtract from the number of words we read
            temp1 = fill_help(token2, dictionary); //We will send to the auxiliary method the word we read that will add it to the dictionary
            add_word_to_probability_list(temp, temp1); //We will send the two words one after the other to the method
            temp = temp1; //Let's move on to the next word by comparing them
        }
    }
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary(LinkList *dictionary)
{
    Node *temp = dictionary->first; //Pointer to the top of the dictionary
    Node *prev;

    while (temp != NULL) //We'll run all over the list
    {
        prev = temp->next; //prev will vote next
        free(temp->data->word); //We will release the word
        free(temp->data->prob_list); //We will release the array
        free(temp->data); //We will release the data
        free(temp); //We will release the voter himself
        temp = prev; //Let's move on to the next limb
    }
    free(dictionary); //At the end we will release the dictionary
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main(int argc, char *argv[])
{

    if (argc < 4 || argc > 5)  //if the arguments that entered are less than 4
    {
        printf("Usage: <seed> <num of tweets> <path> <option: num of words>\n");
        exit(EXIT_FAILURE);
    }

    int seed = atoi(argv[1]); //We will convert to Int to the seed value
    srand(seed); //We'll do a raffle

    int numOfSentences = atoi(argv[2]); //We will convert to Int to the Number of sentences to generate

    FILE *file = fopen(argv[3], "r"); //We will open the file
    if (file == NULL) //check if open the file is succeeded
    {
        printf("Error: opening file failed\n");
        exit(EXIT_FAILURE);
    }

    int numOfWords;
    if (argc == 5) //read according the num in argv[4]
    {
        numOfWords = atoi(argv[4]);
    }
    else //read all the text
    {
        numOfWords = 0;
    }

    LinkList *listDictionary;
    listDictionary = (LinkList*)(malloc(sizeof(LinkList))); //We will make an assignment to the dictionary
    checkFailed(listDictionary); //Check if the assignment was successful
    listDictionary->first = NULL; //The top of the list will point to NULL
    listDictionary->last = NULL; //The end of the list will point to NULL
    listDictionary->size = 0; //The size will be initialized to 0

    fill_dictionary(file, numOfWords, listDictionary); //Is called a method that will fill in the dictionary

    int i;
    for(i = 0; i < numOfSentences; i++) //We will run to the number of tweets we have requested
    {
        printf("Tweet %d: ", i+1);
        generate_sentence(listDictionary); //We'll do a tweet raffle
        printf("\n");
    }

    free_dictionary(listDictionary); //At the end we will release assignments
    fclose (file); //And close the file
    return 0;
}
