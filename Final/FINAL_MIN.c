#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)


// Struct Type들을 정의하는 부분입니다.
typedef struct __clientStruct
{
    int studentNumber;
    char* password;
    char* name;
    char* address;
    char* phoneNumber;

    struct __clientStruct* nextNode;

} clientStruct;
typedef struct __bookStruct
{
    int bookNumber;
    char* bookName;
    char* publisher;
    char* author;
    int ISBN;
    char* location;
    char canBorrow;

    struct __bookStruct* nextNode;
} bookStruct;
typedef struct __borrowStruct
{
    int studentNumber;
    int bookNumber;
    time_t borrowTime;
    time_t returnTime;

    struct __borrowStruct* nextNode;

} borrowStruct;

// 전역 Node 변수들을 선언하는 부분입니다.
clientStruct* clientHeadNode;
clientStruct* clientNode;
clientStruct* tempClientNode;

bookStruct* bookHeadNode;
bookStruct* bookNode;
bookStruct* tempBookNode;
bookStruct* booktail;

borrowStruct* borrowHeadNode;
borrowStruct* borrowNode;
borrowStruct* tempBorrowNode;

// 함수들의 헤더 선언부입니다.
void readFileToStruct_Client();
void readFileToStruct_Book();
void readFileToStruct_Borrow();

void menuPresenter_BookSearch();
void menuPresenter_LibraryMain();

void clientWithdraw();
void showMyRentalList();
void updateMyProfile();
void login();
void signUp();
void registerBook();
void removeBook();
void rentalBook();
void returnBook();
void menuPresenter_BookSearch();
void printClientList();

void searchBookByName();
void searchBookByPublisher();
void searchBookByISBN();
void searchBookByAuthor();
void searchBookByAll();

void saveStringByDynamicAlloc(char** destPtr, char* sourcePtr);
void printLog(char* message);

// 프로그램 메인 실행 부분입니다.
int main(void)
{
    // 파일 불러오기부터 실행합니다.
    readFileToStruct_Book();
    removeBook();
    //registerBook();
    return 0;
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
    booktail = (bookStruct*)(malloc(sizeof(bookStruct)));

    // dynamic alloc for normal node
    bookNode = (bookStruct *)(malloc(sizeof(bookStruct)));

    bookHeadNode = bookNode;
    printf("head : %d\n", bookHeadNode->bookNumber);
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
    printf("head : %d\n", bookHeadNode->bookNumber);
    /* bookNode = bookHeadNode;
    while (bookNode->nextNode != NULL)
    {
        // print list
        printf("%d | %s | %s | %s | %s | %c | %d\n", bookNode->bookNumber, bookNode->bookName, bookNode->publisher, bookNode->author, bookNode->location, bookNode->canBorrow, bookNode->ISBN);
        bookNode = bookNode->nextNode;
    } */
    printLog("Read book.txt success");
}
// book.txt를 bookStruct에 저장하는 함수입니다.

// 도서를 등록하는 함수입니다.
void registerBook()
{
    printLog("도서 등록을 구현해야합니다.");
    int bookNumber;
    char bookName[50];
    char publisher[50];
    char author[50];
    int ISBN;
    char location[50];
    char canBorrow;

    printf("도서명 : ");
    scanf("%s", bookName);
    printf("출판사 : ");
    scanf("%s", publisher);
    printf("저자명 : ");
    scanf("%s", author);
    printf("ISBN : ");
    scanf("%d", &ISBN);
    printf("소장처 : ");
    scanf("%s", location);
    getchar();

    printf("\n자동입력 사항\n\n");
    canBorrow = 'Y';
    bookNumber = booktail->bookNumber +1;
    printf("대여가능 여부 : %c\n",canBorrow);
    printf("도서번호: %d\n\n", bookNumber);

    char check;
    printf("등록하시겠습니까? ");
    scanf("%c", &check);

    bookStruct* newbook = (bookStruct*)malloc(sizeof(bookStruct));
    bookStruct* StpFront = bookHeadNode;
    bookStruct* StpRear = bookHeadNode;
    if (check == 'Y') {
        newbook->ISBN = ISBN;
        newbook->bookNumber = bookNumber;
        newbook->canBorrow = canBorrow;
        saveStringByDynamicAlloc(&(newbook->bookName), bookName);
        saveStringByDynamicAlloc(&(newbook->publisher), publisher);
        saveStringByDynamicAlloc(&(newbook->author), author);
        saveStringByDynamicAlloc(&(newbook->location), location);
        newbook->nextNode = NULL;

        while (0 != StpRear)	//	 삽입 위치  검색 	 //중간 삽입 코드
        {
            if (StpRear->ISBN > newbook->ISBN)	// 삽입 위치 판단 
            {
                break;	// insert location ! 
            }
            StpFront = StpRear;				 //Front = Rear ㅇㅇ
            StpRear = StpRear->nextNode;			 //Rear 가 한칸 이동!
        }
        if (bookHeadNode != StpRear)			 //중간 혹은 끝부분 삽입 시
        {
            newbook->nextNode = StpRear;
            StpFront->nextNode = newbook;
        }
        else			 //앞 삽입

        {
            newbook->nextNode = bookHeadNode;
            bookHeadNode = newbook;
        }
    }
    printf("newbook : %d\n", newbook->ISBN);
    printf("\n등록이 완료되었습니다.\n");
    bookStruct* tempBookHead = bookHeadNode;

            //TODO:추가된 도서 출력 안됨
            printf(">>도서목록<<\n");
            while (tempBookHead != NULL)
            {
                printf("%d. %s \n", tempBookHead->ISBN, tempBookHead->bookName);
                tempBookHead = tempBookHead->nextNode;
            }
}

// 도서를 삭제하는 함수입니다.
void removeBook()
{
    int num,ISBN,bookdeletechecknum=0,booknum;
    char bookName[50];
    char bookdeletecheck;
    bookStruct* bookSeeker = (bookStruct*)(malloc(sizeof(bookStruct)));
    bookSeeker=bookHeadNode;
    printf("bookseeker bookname : %s\n", bookSeeker->bookName);

    printLog(">>도서 삭제<<\n\n");
    printf("1.도서명 검색 2. ISBN 검색\n\n");
    printf("검색 번호를 입력하세요 : ");
    scanf("%d", &num);
    switch (num)
    {
    case 1:
        printf("\n도서명을 입력하세요 : ");
        scanf("%s", bookName);
        printf("bookname : %s", bookName);
        while (booktail != bookSeeker)
        {
            if (strcmp(bookSeeker->bookName, bookName)==0) {
                printf("도서번호 : %d(삭제 가능 여부 : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("도서명 : %s\n", bookSeeker->bookName);
                printf("출판사 : %s\n", bookSeeker->publisher);
                printf("저자명 : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("소장처 : %s\n", bookSeeker->location);
                bookdeletechecknum++;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    case 2:
        printf("\nISBN을 입력하세요 : ");
        scanf("%d", &ISBN);
        while (0 != bookSeeker)
        {
            if (bookSeeker->ISBN== ISBN) {
                printf("도서번호 : %d(삭제 가능 여부 : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("도서명 : %s\n", bookSeeker->bookName);
                printf("출판사 : %s\n", bookSeeker->publisher);
                printf("저자명 : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("소장처 : %s\n", bookSeeker->location);
                if (bookSeeker->canBorrow == 'Y') {
                    bookdeletechecknum++;
                }
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    }
    if (bookdeletechecknum > 0) {
        printf("\n삭제할 도서번호를 입력하세요 : ");
        scanf("%d", &booknum);
        getchar();
        printf("\n\n이 도서를 삭제합니까? : ");
        scanf("%c", &bookdeletecheck);
        if (bookdeletecheck == 'Y') {
            //도서삭제코드
            bookStruct* StpFront = bookHeadNode;
            bookStruct* StpRear = bookHeadNode;
            while (0 != StpRear)	//	 삽입 위치  검색 	 //중간 삽입 코드
            {
                if (StpRear->bookNumber== booknum)	// 삽입 위치 판단 
                {
                    break;	// insert location ! 
                }
                StpFront = StpRear;				 //Front = Rear
                StpRear = StpRear->nextNode;			 //Rear 가 한칸 이동
            }
            if (bookHeadNode != StpRear)			 //중간 혹은 끝부분 삽입 시
            {

                //TODO: 중간부분 삽입 코드

            }
            else if (bookHeadNode == StpRear)	 // Head -> booknum 을 지울 때
            {
                bookHeadNode = bookHeadNode->nextNode;
                free(StpRear);
            }
            else if (0 == StpRear)
            {
                printf("잘못된 입력입니다\n");
            }
            printf("\n\n도서가 삭제 되었습니다.\n\n ");
        }
     }
    bookStruct* tempBookHead = bookHeadNode;
    printf(">>도서목록<<\n");
    while (tempBookHead != NULL)
    {
        printf("%d. %s \n", tempBookHead->ISBN, tempBookHead->bookName);
        tempBookHead = tempBookHead->nextNode;
    }
}

// 도서를 대여하는 함수입니다.
void rentalBook()
{
    int num,ISBN,studentnum, booknum,bookborrowchecknum=0;
    char bookName[50];
    char bookborrowcheck;
    bookStruct* bookSeeker = (bookStruct*)(malloc(sizeof(bookStruct)));
    bookSeeker = bookHeadNode;
    printLog(">>도서 대여<<\n\n");
    printf("1.도서명 검색 2. ISBN 검색\n\n");
    printf("검색 번호를 입력하세요 : ");
    scanf("%d", &num);
    switch (num)
    {
    case 1:
        printf("\n도서명을 입력하세요 : ");
        scanf("%s", bookName);
        printf("bookname : %s", bookName);
        while (booktail != bookSeeker)
        {
            if (strcmp(bookSeeker->bookName, bookName) == 0) {
                printf(">>검색결과<<\n");
                printf("도서번호 : %d(대여 가능 여부 : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("도서명 : %s\n", bookSeeker->bookName);
                printf("출판사 : %s\n", bookSeeker->publisher);
                printf("저자명 : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("소장처 : %s\n", bookSeeker->location);
                bookborrowchecknum++;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    case 2:
        printf("\nISBN을 입력하세요 : ");
        scanf("%d", &ISBN);
        while (0 != bookSeeker)
        {
            if (bookSeeker->ISBN == ISBN) {
                printf("도서번호 : %d(대여 가능 여부 : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("도서명 : %s\n", bookSeeker->bookName);
                printf("출판사 : %s\n", bookSeeker->publisher);
                printf("저자명 : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("소장처 : %s\n", bookSeeker->location);
                bookborrowchecknum++;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    }
    if (bookborrowchecknum > 0) {
        printf("\n학번을 입력하세요 : ");
        scanf("%d", &studentnum);
        printf("\n도서번호를 입력하세요 : ");
        scanf("%d", &booknum);
        getchar();
        printf("\n\n이 도서를 대여합니까? : ");
        scanf("%c", &bookborrowcheck);
        if (bookborrowcheck == 'Y') {
            //TODO : 학생의 도서 대여 정보 수정
            printf("\n\n도서가 대여 되었습니다.\n\n ");
        }
    }
    // TODO: 민비 & 세연님
}

// 도서를 반납하는 함수입니다.
void returnBook()
{
    printLog("도서 반납을 구현해야합니다.");
    // TODO: 민비 & 세연님
}
// 입력받은 string을 동적 할당으로 저장하는 함수입니다.
void saveStringByDynamicAlloc(char** destPtr, char* sourcePtr)
{
    *destPtr = (char*)malloc(strlen(sourcePtr) + 1);
    strcpy(*destPtr, sourcePtr);
}

// 개발용 로그를 출력하는 함수입니다.
// 자동 줄바꿈 처리가 되므로, 개행문자는 입력하지 않는게 좋습니다.
// 프로젝트 마무리 후 모든 관련 항목들을 제거해야합니다.
void printLog(char** message)
{
    printf("\n[LOG] %s\n", message);
}