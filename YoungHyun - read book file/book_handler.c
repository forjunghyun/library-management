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

    // dynamic alloc for head node
    bookStruct *bookHeadNode = (bookStruct *)(malloc(sizeof(bookStruct)));

    // dynamic alloc for normal node
    bookStruct *bookNode = (bookStruct *)(malloc(sizeof(bookStruct)));

    bookHeadNode = bookNode;

    // File handling
    FILE *bookFileReadStream = fopen("book.txt", "r");

    // loop - read until encountering new line
    while ((fscanf(bookFileReadStream, "%[^\n]", tempString)) != EOF)
    {
        // Cut the string and put it into book structure
        bookNode->bookNumber = atoi(strtok(tempString, "|"));
        bookNode->bookName = strtok(NULL, "|");
        bookNode->publisher = strtok(NULL, "|");
        bookNode->author = strtok(NULL, "|");
        bookNode->ISBN = atoi(strtok(NULL, "|"));
        bookNode->location = strtok(NULL, "|");
        bookNode->canBorrow = strtok(NULL, "|")[0];

        // printf("%d | %s | %s | %s | %s | %c | %d\n", bookNode->bookNumber, bookNode->bookName, bookNode->publisher, bookNode->author, bookNode->location, bookNode->canBorrow, bookNode->ISBN);

        // dynamic alloc for next node
        // move node index to next node
        bookStruct *tempBookNode = (bookStruct *)(malloc(sizeof(bookStruct)));
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
        printf("%d | %s | %s | %s | %s | %c | %d\n", bookNode->bookNumber, bookNode->bookName, bookNode->publisher, bookNode->author, bookNode->location, bookNode->canBorrow, bookNode->ISBN);
        // printf("%s\n", bookNode->publisher);
        bookNode = bookNode->nextNode;
    }
}