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

int main(void)
{
    // Temp string for line input
    char tempString[999];
    char *tempTokPtr;

    // dynamic alloc for head node
    bookStruct *bookHeadNode = (bookStruct *)(malloc(sizeof(bookStruct)));

    // dynamic alloc for normal node
    bookStruct *bookNode = (bookStruct *)(malloc(sizeof(bookStruct)));
    bookStruct *tempBookNode;

    bookHeadNode = bookNode;

    // File handling
    FILE *bookFileReadStream = fopen("book.txt", "r");

    // loop - read until encountering new line
    while ((fscanf(bookFileReadStream, "%[^\n]", tempString)) != EOF)
    {
        // Cut the string and put it into book structure
        // handle bookNumber
        bookNode->bookNumber = atoi(strtok(tempString, "|"));

        // handle bookName
        tempTokPtr = strtok(NULL, "|");
        bookNode->bookName = (char *)malloc(strlen(tempTokPtr) + 1);
        strcpy(bookNode->bookName, tempTokPtr);

        // handle publisher
        tempTokPtr = strtok(NULL, "|");
        bookNode->publisher = (char *)malloc(strlen(tempTokPtr) + 1);
        strcpy(bookNode->publisher, tempTokPtr);

        // handle author
        tempTokPtr = strtok(NULL, "|");
        bookNode->author = (char *)malloc(strlen(tempTokPtr) + 1);
        strcpy(bookNode->author, tempTokPtr);

        bookNode->ISBN = atoi(strtok(NULL, "|"));

        // handle location
        tempTokPtr = strtok(NULL, "|");
        bookNode->location = (char *)malloc(strlen(tempTokPtr) + 1);
        strcpy(bookNode->location, tempTokPtr);

        // handle canBorrow
        bookNode->canBorrow = strtok(NULL, "|")[0];

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