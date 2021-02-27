#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __bookStruct
{
    int bookNumber;
    char *bookName;
    char *publisher;
    char *author;
    int ISBN;
    char *location;
    char canBorrow;

    struct __bookStruct *nextNode;
} bookStruct;

void saveStringByDynamicAlloc(char **, char *);

int main(void)
{
    // temp strings for line input
    char tempString[999];
    char *tempTokPtr;

    // node to save first node.
    bookStruct *bookHeadNode = (bookStruct *)(malloc(sizeof(bookStruct)));

    // dynamic alloc for normal node
    bookStruct *bookNode = (bookStruct *)(malloc(sizeof(bookStruct)));
    bookStruct *tempBookNode;

    bookHeadNode = bookNode;

    // file handling
    FILE *bookFileReadStream = fopen("book.txt", "r");

    // loop - read until encountering new line
    while ((fscanf(bookFileReadStream, "%[^\n]", tempString)) != EOF)
    {
        // Cut the string and put it into book structure
        // saves integer values using atoi
        // saves string values using strcpy after malloc function
        bookNode->bookNumber = atoi(strtok(tempString, "|"));
        saveStringByDynamicAlloc(&(bookNode->bookName), strtok(NULL, "|"));
        saveStringByDynamicAlloc(&(bookNode->publisher), strtok(NULL, "|"));
        saveStringByDynamicAlloc(&(bookNode->author), strtok(NULL, "|"));
        bookNode->ISBN = atoi(strtok(NULL, "|"));
        saveStringByDynamicAlloc(&(bookNode->location), strtok(NULL, "|"));
        bookNode->canBorrow = strtok(NULL, "|")[0]; // no need for dynamic alloc

        // dynamic alloc for next node
        // move node index to next node
        tempBookNode = (bookStruct *)(malloc(sizeof(bookStruct)));
        bookNode->nextNode = tempBookNode;
        bookNode = tempBookNode;

        bookNode->nextNode = NULL;

        fgetc(bookFileReadStream); // Reads in '\n' character and moves file
                                   // stream past delimiting character
    }
    fclose(bookFileReadStream);

    // set node index to first node.
    bookNode = bookHeadNode;
    while (bookNode->nextNode != NULL)
    {
        // print list
        printf("%d | %s | %s | %s | %s | %c | %d\n", bookNode->bookNumber, bookNode->bookName, bookNode->publisher, bookNode->author, bookNode->location, bookNode->canBorrow, bookNode->ISBN);
        bookNode = bookNode->nextNode;
    }
}

// copy source string to destPtr, with dynamic allocation using malloc
void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr)
{
    *destPtr = (char *)malloc(strlen(sourcePtr) + 1);
    strcpy(*destPtr, sourcePtr);
}
