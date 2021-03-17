#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

// time_t를 정의하는 부분입니다.
typedef long int time_t;

// Struct Type들을 정의하는 부분입니다.
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
typedef struct __borrowStruct
{
    int studentNumber;
    int bookNumber;
    time_t borrowTime;
    time_t returnTime;

    struct __borrowStruct *nextNode;

} borrowStruct;

// 전역 Node 변수들을 선언하는 부분입니다.
clientStruct *clientHeadNode;
clientStruct *clientNode;
clientStruct *tempClientNode;

bookStruct *bookHeadNode;
bookStruct *bookNode;
bookStruct *tempBookNode;

borrowStruct *borrowHeadNode;
borrowStruct *borrowNode;
borrowStruct *tempBorrowNode;

// 함수들의 헤더 선언부입니다.
void readFileToStruct_Client();
void readFileToStruct_Book();
void readFileToStruct_Borrow();

void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr);
void printLog(char *message);

// 프로그램 메인 실행 부분입니다.
int main()
{
    // 파일 불러오기부터 실행합니다.
    readFileToStruct_Client();
    readFileToStruct_Book();
    readFileToStruct_Borrow();

    // 불러오기가 성공하면 로그인 해야합니다.
    
    return 0;
}

// client.txt를 clientStruct에 저장하는 함수입니다.
void readFileToStruct_Client()

{
    char tempString[999];
    char *tempTokPtr;

    clientHeadNode = (clientStruct *)(malloc(sizeof(clientStruct)));
    clientNode = (clientStruct *)(malloc(sizeof(clientStruct)));

    clientHeadNode = clientNode;

    FILE *clientFileReadStream = fopen("client.txt", "r");

    while ((fscanf(clientFileReadStream, "%[^\n]", tempString)) != EOF)
    {
        clientNode->studentNumber = atoi(strtok(tempString, "|"));
        saveStringByDynamicAlloc(&(clientNode->password), strtok(NULL, "|"));
        saveStringByDynamicAlloc(&(clientNode->name), strtok(NULL, "|"));
        saveStringByDynamicAlloc(&(clientNode->address), strtok(NULL, "|"));
        saveStringByDynamicAlloc(&(clientNode->phoneNumber), strtok(NULL, "|"));

        tempClientNode = (clientStruct *)(malloc(sizeof(clientStruct)));
        clientNode->nextNode = tempClientNode;
        clientNode = tempClientNode;

        clientNode->nextNode = NULL;

        fgetc(clientFileReadStream);
    }
    fclose(clientFileReadStream);

    // 잘 입력이 됐는지 출력하는 부분입니다.
    /* clientNode = clientHeadNode;
    while (clientNode->nextNode != NULL)
    {
        printf("%d | %s | %s | %s | %s\n", clientNode->studentNumber, clientNode->password, clientNode->name, clientNode->address, clientNode->phoneNumber);
        clientNode = clientNode->nextNode;
    } */
    printLog("Read client.txt success");
}

// book.txt를 bookStruct에 저장하는 함수입니다.
void readFileToStruct_Book()
{
    FILE *book_file = fopen("book.txt", "r");

    // temp strings for line input
    char book_tempString[999];
    char *book_tempTokPtr;

    // node to save first node.
    bookHeadNode = (bookStruct *)(malloc(sizeof(bookStruct)));
    tempBookNode = (bookStruct *)(malloc(sizeof(bookStruct)));

    // dynamic alloc for normal node
    bookNode = (bookStruct *)(malloc(sizeof(bookStruct)));

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

        tempBookNode = bookNode;

        bookNode->nextNode = tempBookNode;
        bookNode = tempBookNode;

        bookNode->nextNode = NULL;

        fgetc(book_file); // Reads in '\n' character and moves file
                          // stream past delimiting character
    }
    fclose(book_file);

    /* bookNode = bookHeadNode;
    while (bookNode->nextNode != NULL)
    {
        // print list
        printf("%d | %s | %s | %s | %s | %c | %d\n", bookNode->bookNumber, bookNode->bookName, bookNode->publisher, bookNode->author, bookNode->location, bookNode->canBorrow, bookNode->ISBN);
        bookNode = bookNode->nextNode;
    } */
    printLog("Read book.txt success");
}

// borrow.txt를 borrowStruct에 저장하는 함수입니다.
void readFileToStruct_Borrow()
{
    FILE *borrow_file = fopen("borrow.txt", "r");
    char borrow_tempString[999];

    borrowHeadNode = (borrowStruct *)malloc(sizeof(borrowStruct));
    borrowNode = (borrowStruct *)malloc(sizeof(borrowStruct));

    borrowHeadNode = borrowNode;

    char *token;

    while (1)
    {
        fscanf(borrow_file, "%s", borrow_tempString);
        if (feof(borrow_file))
            break;
        token = strtok(borrow_tempString, "|");
        borrowNode->studentNumber = atoi(token);
        token = strtok(NULL, "| ");
        borrowNode->bookNumber = atoi(token);
        token = strtok(NULL, "| ");
        borrowNode->borrowTime = atoi(token);
        token = strtok(NULL, "");
        borrowNode->returnTime = atoi(token);

        tempBorrowNode = (borrowStruct *)malloc(sizeof(borrowStruct));

        borrowNode->nextNode = tempBorrowNode;
        borrowNode = tempBorrowNode;

        borrowNode->nextNode = NULL;
    }
    fclose(borrow_file);

    /*  borrowNode = borrowHeadNode;
    while (borrowNode->nextNode != NULL)
    {
        printf("%d | %08d | %ld | %ld\n", borrowNode->studentNumber, borrowNode->bookNumber, borrowNode->borrowTime, borrowNode->returnTime);
        borrowNode = borrowNode->nextNode;
    } */
    printLog("Read borrow.txt success");
}

// 동적 할당으로 입력받은 string을 저장하는 함수입니다.
void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr)
{
    *destPtr = (char *)malloc(strlen(sourcePtr) + 1);
    strcpy(*destPtr, sourcePtr);
}

// 추후 제거해야 할, 개발용 로그를 출력하는 함수입니다.
// 자동 줄바꿈 처리가 되므로, 개행문자는 입력하지 않는게 좋습니다.
void printLog(char *message)
{
    printf("\n[LOG] %s\n", message);
}