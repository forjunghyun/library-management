#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define time_t int
#pragma warning(disable : 4996)
typedef struct __clientStruct
{
	int studentNumber;
	char *password;
	char *name;
	char *address;
	char *phoneNumber;

	struct __clientStruct *nextNode;

} clientStruct;
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
typedef struct Borrow
{
	int studentNumber;
	int bookNumber;
	time_t borrowTime;
	time_t returnTime;

	struct Borrow *next;

} Borrow;

clientStruct *clientHeadNode;
clientStruct *clienttail;
bookStruct *bookHeadNode;
bookStruct *booktail;
Borrow *BorrowHeadNode;
Borrow *Borrowtail;

void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr);
void file_read();

int main()
{
	file_read();

	return 0;
}
void file_read()
{
	FILE *client_file, *book_file, *borrow_file;
	//client ����
	char client_tempString[999];
	char *client_tempTokPtr;

	clientHeadNode = (clientStruct *)(malloc(sizeof(clientStruct)));
	clienttail = (clientStruct *)(malloc(sizeof(clientStruct)));
	clientStruct *clientNode = (clientStruct *)(malloc(sizeof(clientStruct)));
	clientStruct *tempClientNode;

	clientHeadNode = clientNode;

	client_file = fopen("client.txt", "r");

	while ((fscanf(client_file, "%[^\n]", client_tempString)) != EOF)
	{
		clientNode->studentNumber = atoi(strtok(client_tempString, "|"));
		saveStringByDynamicAlloc(&(clientNode->password), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->name), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->address), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->phoneNumber), strtok(NULL, "|"));

		tempClientNode = (clientStruct *)(malloc(sizeof(clientStruct)));

		clienttail = clientNode;

		clientNode->nextNode = tempClientNode;
		clientNode = tempClientNode;

		clientNode->nextNode = NULL;

		fgetc(client_file);
	}

	clientNode = clientHeadNode;

	fclose(client_file);

	while (clientNode->nextNode != NULL)
	{
		printf("%d | %s | %s | %s | %s\n", clientNode->studentNumber, clientNode->password, clientNode->name, clientNode->address, clientNode->phoneNumber);
		clientNode = clientNode->nextNode;
	}
	printf("head : %d\n", clientHeadNode->studentNumber);
	printf("tail : %d\n", clienttail->studentNumber);

	book_file = fopen("book.txt", "r");

	// temp strings for line input
	char book_tempString[999];
	char *book_tempTokPtr;

	// node to save first node.
	bookHeadNode = (bookStruct *)(malloc(sizeof(bookStruct)));
	booktail = (bookStruct *)(malloc(sizeof(bookStruct)));

	// dynamic alloc for normal node
	bookStruct *bookNode = (bookStruct *)(malloc(sizeof(bookStruct)));
	bookStruct *tempBookNode;

	bookHeadNode = bookNode;

	// loop - read until encountering new line
	while ((fscanf(book_file, "%[^\n]", book_tempString)) != EOF)
	{
		// Cut the string and put it into book structure
		// saves integer values using atoi
		// saves string values using strcpy after malloc function
		bookNode->bookNumber = atoi(strtok(book_tempString, "|"));
		saveStringByDynamicAlloc(&(bookNode->bookName), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(bookNode->publisher), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(bookNode->author), strtok(NULL, "|"));
		bookNode->ISBN = atoi(strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(bookNode->location), strtok(NULL, "|"));
		bookNode->canBorrow = strtok(NULL, "|")[0]; // no need for dynamic alloc

		// dynamic alloc for next node
		// move node index to next node
		tempBookNode = (bookStruct *)(malloc(sizeof(bookStruct)));

		booktail = bookNode;

		bookNode->nextNode = tempBookNode;
		bookNode = tempBookNode;

		bookNode->nextNode = NULL;

		fgetc(book_file); // Reads in '\n' character and moves file
						  // stream past delimiting character
	}
	fclose(book_file);

	// set node index to first node.
	bookNode = bookHeadNode;
	while (bookNode->nextNode != NULL)
	{
		// print list
		printf("%d | %s | %s | %s | %s | %c | %d\n", bookNode->bookNumber, bookNode->bookName, bookNode->publisher, bookNode->author, bookNode->location, bookNode->canBorrow, bookNode->ISBN);
		bookNode = bookNode->nextNode;
	}

	printf("head : %d\n", bookHeadNode->bookNumber);
	printf("tail : %d\n", booktail->bookNumber);

	//borrow ����
	borrow_file = fopen("borrow.txt", "r");
	char borrow_tempString[999];
	char *borrow_tempTokPtr;

	BorrowHeadNode = (Borrow *)malloc(sizeof(Borrow));
	Borrowtail = (Borrow *)(malloc(sizeof(Borrow)));
	Borrow *BorrowNode = (Borrow *)malloc(sizeof(Borrow));
	Borrow *tempBorrowNode;

	BorrowHeadNode = BorrowNode;

	char *token;

	while (1)
	{
		fscanf(borrow_file, "%s", borrow_tempString);
		if (feof(borrow_file))
		{
			break;
		}

		token = strtok(borrow_tempString, "|");
		BorrowNode->studentNumber = atoi(token);
		token = strtok(NULL, "| ");
		BorrowNode->bookNumber = atoi(token);
		token = strtok(NULL, "| ");
		BorrowNode->borrowTime = atoi(token);
		token = strtok(NULL, "");
		BorrowNode->returnTime = atoi(token);

		tempBorrowNode = (Borrow *)malloc(sizeof(Borrow));

		Borrowtail = BorrowNode;

		BorrowNode->next = tempBorrowNode;
		BorrowNode = tempBorrowNode;

		BorrowNode->next = NULL;

		/*fgetc(borrow_file);*/
	}

	BorrowNode = BorrowHeadNode;

	fclose(borrow_file);

	while (BorrowNode->next != NULL)
	{
		printf("%d | %08d | %d | %d\n", BorrowNode->studentNumber, BorrowNode->bookNumber, BorrowNode->borrowTime, BorrowNode->returnTime);
		BorrowNode = BorrowNode->next;
	}

	printf("head : %d\n", BorrowHeadNode->studentNumber);
	printf("tail : %d\n", Borrowtail->studentNumber);
}

void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr)
{
	*destPtr = (char *)malloc(strlen(sourcePtr) + 1);
	strcpy(*destPtr, sourcePtr);
}
void linkedlist_store()
{

	return;
}