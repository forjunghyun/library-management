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

void menuPresenter_LibraryMain();

void signUp();

void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr);
void printLog(char *message);

// 프로그램 메인 실행 부분입니다.
int main(void)
{
    // 파일 불러오기부터 실행합니다.
    readFileToStruct_Client();
    readFileToStruct_Book();
    readFileToStruct_Borrow();

    // 첫 불러오기가 성공하면 메인페이지로 넘어갑니다.
    menuPresenter_LibraryMain();
    return 0;
}

// ************************************************************
// * 파일과 구조체를 읽고 쓰는, 입출력 함수들이 모여있는 곳   *
// * 입니다.                                                  *
// ************************************************************

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

// ************************************************************
// * 메모리상의 구조체의 내용을 출력하는 함수들입니다.        *
// ************************************************************

void printStruct_Client()
{
    clientNode = clientHeadNode;
    while (clientNode != NULL)
    {
        printf("%d | %s | %s | %s | %s\n", clientNode->studentNumber, clientNode->password, clientNode->name, clientNode->address, clientNode->phoneNumber);
        clientNode = clientNode->nextNode;
    }
}

// ************************************************************
// * 메인페이지, 일반 회원 페이지, 관리자 페이지 등 페이지의  *
// * 출력부들이 모여있는 곳입니다.                            *
// ************************************************************

// 프로그램 첫 실행 후 출력되는 도서관 서비스 안내 페이지입니다.
// 화면 출력의 의미인 Presenter로 작명하였습니다.
void menuPresenter_LibraryMain()
{
    // 사용자가 입력한 메뉴 번호를 담는 변수입니다.
    short menuSelected;

    // 잘못 입력하거나, 로그아웃 시에 다시 돌아와야합니다.
    while (1)
    {
        printf("\n>>도서관 서비스<<\n");
        printf("1. 회원 가입\n");
        printf("2. 로그인\n");
        printf("3. 프로그램 종료\n\n");
        printf("번호를 선택하세요: ");

        scanf("%hd", &menuSelected);
        switch (menuSelected)
        {
        case 1:
            signUp();
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;

        default:
            break;
        }
    }
}

// ************************************************************
// * 회원가입, 로그인 등 가장 중요한 로직이 모여있는 곳입니다.*
// ************************************************************

// 회원가입을 담당하는 함수입니다.
void signUp()
{
    // 입력받은 string을 동적 할당하기 전, 잠시 저장해 놓는
    // 변수들입니다.
    int studentNumber;
    char password[100];
    char name[100];
    char address[100];
    char phoneNumber[100];

    printf("\n< 회원가입 > \n\n");
    printf("학번 : ");
    scanf("%d", &studentNumber);
    printf("비밀번호 : ");
    scanf("%s", password);
    printf("이름 : ");
    scanf("%s", name);
    printf("주소 : ");
    scanf("%s", address);
    printf("전화번호 : ");
    scanf("%s", phoneNumber);

    // 입력받은 값들을 새로운 clientStruct에 저장합니다.
    // 문자열들은 동적 할당으로 저장합니다.
    clientStruct *newClient = (clientStruct *)malloc(sizeof(clientStruct));
    newClient->studentNumber = studentNumber;
    saveStringByDynamicAlloc(&(newClient->password), password);
    saveStringByDynamicAlloc(&(newClient->name), name);
    saveStringByDynamicAlloc(&(newClient->address), address);
    saveStringByDynamicAlloc(&(newClient->phoneNumber), phoneNumber);
    newClient->nextNode = NULL;

    // 세연님이 올려주신, clientNode 중간에 새로운 유저를 삽입하는
    // 코드입니다.
    clientStruct *StpFront = clientHeadNode;
    clientStruct *StpRear = clientHeadNode;
    while (StpRear != 0) //	삽입 위치 검색 - 중간 삽입 코드
    {
        if (StpRear->studentNumber > newClient->studentNumber) // 삽입 위치 판단
        {
            break; // insert location !
        }
        StpFront = StpRear;          // Front = Rear ㅇㅇ
        StpRear = StpRear->nextNode; // Rear 가 한칸 이동!
    }
    if (clientHeadNode != StpRear) // 중간 혹은 끝부분 삽입 시
    {
        newClient->nextNode = StpRear;
        StpFront->nextNode = newClient;
    }
    else // 앞 삽입
    {
        newClient->nextNode = clientHeadNode;
        clientHeadNode = newClient;
    }

    printf("\n회원가입이 완료되었습니다.\n");
    printStruct_Client();
}

// ************************************************************
// * 실제 로직에 직접 연관이 있지 않은, string 저장과 같은  *
// * 유용한 함수들이 모여있는 곳입니다.                       *
// ************************************************************

// 입력받은 string을 동적 할당으로 저장하는 함수입니다.
void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr)
{
    *destPtr = (char *)malloc(strlen(sourcePtr) + 1);
    strcpy(*destPtr, sourcePtr);
}

// 개발용 로그를 출력하는 함수입니다.
// 자동 줄바꿈 처리가 되므로, 개행문자는 입력하지 않는게 좋습니다.
// 프로젝트 마무리 후 모든 관련 항목들을 제거해야합니다.
void printLog(char *message)
{
    printf("\n[LOG] %s\n", message);
}