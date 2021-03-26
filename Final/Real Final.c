#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define ONE_DAY (1 * 60 * 60 * 24)                                    // 1일을 초로
#define UTC_TIME_WEEKDAY_OFFSET (4) /* 1970,1,1은 목요일이기때문에 */ // UTC 시작 시간

typedef uint32_t timestamp_t; //seconds, unix시간 seconds 부분 다시 재정의한 부분

//날짜                    x, 1월, 2월 ..... 11월, 12월
uint8_t month_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Struct Type들을 정의하는 부분입니다.
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t week;
    uint8_t weekday;
} datetime_t;
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
    timestamp_t borrowTime;
    timestamp_t returnTime;

    struct __borrowStruct *nextNode;

} borrowStruct;

// 전역 Node 변수들을 선언하는 부분입니다.
clientStruct *clientHeadNode;
clientStruct *clientNode;
clientStruct *tempClientNode;
clientStruct *clientLoggedIn;
clientStruct *clientTail;

bookStruct *bookHeadNode;
bookStruct *bookNode;
bookStruct *tempBookNode;
bookStruct *booktail;

borrowStruct *borrowHeadNode;
borrowStruct *borrowNode;
borrowStruct *tempBorrowNode;

// 함수들의 헤더 선언부입니다.
void readFileToStruct_Client();
void readFileToStruct_Book();
void readFileToStruct_Borrow();

void saveFileToStruct_Client();
void saveFileToStruct_Book();
void saveFileToStruct_Borrow();

void menuPresenter_BookSearch();
void menuPresenter_LibraryMain();
void menuPresenter_Administrator();
void menuPresenter_NormalClient();

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

void saveAndExit();
void saveStringByDynamicAlloc(char **destPtr, char *sourcePtr);
uint8_t timestamp_to_weekday(timestamp_t timestamp_sec);
int is_leap_year(uint16_t year);
void utc_timestamp_to_date(timestamp_t timestamp, datetime_t *datetime);
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
    clientNode = clientHeadNode;
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
    booktail = (bookStruct *)(malloc(sizeof(bookStruct)));

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

        booktail = bookNode;

        bookNode->nextNode = tempBookNode;
        bookNode = tempBookNode;

        bookNode->nextNode = NULL;

        fgetc(book_file); // Reads in '\n' character and moves file
                          // stream past delimiting character
    }
    fclose(book_file);
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
}

// ************************************************************
// * 메모리상의 구조체의 내용을 txt로 저장하는 곳입니다.      *
// ************************************************************
void saveFileToStruct_Client()
{
    FILE *clientFileWriteStream = fopen("client.txt", "w");
    clientNode = clientHeadNode;
    while (clientNode->nextNode != NULL)
    {
        fprintf(clientFileWriteStream, "%d|%s|%s|%s|%s\n", clientNode->studentNumber, clientNode->password, clientNode->name, clientNode->address, clientNode->phoneNumber);
        clientNode = clientNode->nextNode;
    }
    fclose(clientFileWriteStream);
    printf("\n사용자 목록 저장이 성공적으로 완료되었습니다.\n");
}

void saveFileToStruct_Book()
{
    FILE *bookFileWriteStream = fopen("book.txt", "w");
    bookNode = bookHeadNode;
    while (bookNode != booktail)
    {
        // bookNumber, ISBN은 각각 7, 13자리 입니다.
        fprintf(bookFileWriteStream, "%07d|%s|%s|%s|%013d|%s|%c\n", bookNode->bookNumber, bookNode->bookName, bookNode->publisher, bookNode->author, bookNode->ISBN, bookNode->location, bookNode->canBorrow);
        bookNode = bookNode->nextNode;
    }
    fclose(bookFileWriteStream);
    printf("도서 목록 저장이 성공적으로 완료되었습니다.\n");
}

void saveFileToStruct_Borrow()
{
    FILE *borrowFileWriteStream = fopen("borrow.txt", "w");
    borrowNode = borrowHeadNode;
    while (borrowNode->nextNode != NULL)
    {
        fprintf(borrowFileWriteStream, "%d|%07d|%d|%d\n", borrowNode->studentNumber, borrowNode->bookNumber, borrowNode->borrowTime, borrowNode->returnTime);
        borrowNode = borrowNode->nextNode;
    }
    fclose(borrowFileWriteStream);
    printf("대여 목록 저장이 성공적으로 완료되었습니다.\n");
}

// ************************************************************
// * 메모리상의 구조체의 내용을 출력하는 함수들입니다.        *
// ************************************************************

void printStruct_Client()
{
    clientNode = clientHeadNode;
    while (clientNode->nextNode != NULL)
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
            login();
            break;
        case 3:
            saveAndExit();
            break;

        default:
            printf("잘못 입력하셨습니다.");
            break;
        }
    }
}

// 일반 회원 페이지를 출력하는 함수입니다.
void menuPresenter_NormalClient()
{
    // 사용자가 입력한 메뉴 번호를 담는 변수입니다.
    short menuSelected;

    // 잘못 입력하거나, 로그아웃 시에 다시 돌아와야합니다.
    while (1)
    {
        printf("\n>>회원 메뉴<<\n");
        printf("1. 도서 검색\n");
        printf("2. 내 대여 목록\n");
        printf("3. 개인정보 수정\n");
        printf("4. 회원 탈퇴\n");
        printf("5. 로그아웃\n");
        printf("6. 프로그램 종료\n\n");
        printf("번호를 선택하세요: ");

        scanf("%hd", &menuSelected);
        switch (menuSelected)
        {
        case 1:
            menuPresenter_BookSearch();
            break;
        case 2:
            showMyRentalList();
            break;
        case 3:
            updateMyProfile();
            break;
        case 4:
            clientWithdraw();
            break;
        case 5:
            return;
            break;
        case 6:
            saveAndExit();
            break;
        default:
            printf("잘못 입력하셨습니다.");
            break;
        }
    }
}

// 관리자 페이지를 출력하는 함수입니다.
void menuPresenter_Administrator()
{
    // 사용자가 입력한 메뉴 번호를 담는 변수입니다.
    short menuSelected;

    // 잘못 입력하거나, 로그아웃 시에 다시 돌아와야합니다.
    while (1)
    {
        printf("\n>>회원 메뉴<<\n");
        printf("1. 도서 등록\n");
        printf("2. 도서 삭제\n");
        printf("3. 도서 대여\n");
        printf("4. 도서 반납\n");
        printf("5. 도서 검색\n");
        printf("6. 회원 목록\n");
        printf("7. 로그아웃\n");
        printf("8. 프로그램 종료\n\n");
        printf("번호를 선택하세요: ");

        scanf("%hd", &menuSelected);
        switch (menuSelected)
        {
        case 1:
            registerBook();
            break;
        case 2:
            removeBook();
            break;
        case 3:
            rentalBook();
            break;
        case 4:
            returnBook();
            break;
        case 5:
            menuPresenter_BookSearch();
            break;
        case 6:
            printClientList();
            break;
        case 7:
            return;
            break;
        case 8:
            saveAndExit();
            break;
        default:
            printf("잘못 입력하셨습니다.");
            break;
        }
    }
}

// 책 검색 페이지를 출력하는 함수입니다.
void menuPresenter_BookSearch()
{
    // 사용자가 입력한 메뉴 번호를 담는 변수입니다.
    short menuSelected;

    // 잘못 입력하거나, 로그아웃 시에 다시 돌아와야합니다.
    while (1)
    {
        printf("\n>>도서 검색<<\n");
        printf("1. 도서명 검색\n");
        printf("2. 출판사 검색\n");
        printf("3. ISBN 검색\n");
        printf("4. 저자명 검색\n");
        printf("5. 전체 목록\n");
        printf("6. 이전 메뉴\n\n");
        printf("번호를 선택하세요: ");

        scanf("%hd", &menuSelected);
        switch (menuSelected)
        {
        case 1:
            searchBookByName();
            break;
        case 2:
            searchBookByPublisher();
            break;
        case 3:
            searchBookByISBN();
            break;
        case 4:
            searchBookByAuthor();
            break;
        case 5:
            searchBookByAll();
            break;
        case 6:
            return;
            break;
        default:
            printf("잘못 입력하셨습니다.");
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
    while (StpRear->nextNode != NULL) //	삽입 위치 검색 - 중간 삽입 코드
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

// 로그인을 담당하는 함수입니다.
void login()
{
    int tempInt;
    char tempString[50];
    short isValidUser;
    clientStruct *tempNode = clientHeadNode;

    isValidUser = 0;
    printf("로그인명(학번) : ");
    scanf("%s", tempString);

    // 관리자인지 일단 먼저 검사
    if (strcmp(tempString, "admin") == 0)
    {
        menuPresenter_Administrator();
    }
    else
    {
        // 학번 (아이디)를 입력받음.
        tempInt = atoi(tempString);
        printf("비밀번호 입력 : ");
        scanf("%s", tempString);

        // 모든 client 노드들을 순환해서 검사.
        while (tempNode->nextNode != NULL)
        {
            if (tempNode->studentNumber == tempInt && strcmp(tempNode->password, tempString) == 0)
            {
                clientLoggedIn = tempNode;
                isValidUser = 1;
                break;
            }
            else
            {
                tempNode = tempNode->nextNode;
            }
        }

        // 추후 로그아웃 했을 때 되돌아오기 위해 isValidUser 사용.
        if (isValidUser)
        {
            menuPresenter_NormalClient();
            printf("로그아웃이 완료되었습니다.\n\n");
        }
        else
        {
            printf("없는 회원입니다.\n\n");
        }
    }
}

// 내 도서 대여 목록을 보여주는 함수입니다.
void showMyRentalList()
{
    timestamp_t unix_timestamp1;
    timestamp_t unix_timestamp2;
    datetime_t datetime1;
    datetime_t datetime2;

    borrowStruct *tempHead = borrowHeadNode;
    printf(">>내 대여 도서 목록<<\n");
    while (tempHead->nextNode != NULL)
    {
        if (tempHead->studentNumber == clientLoggedIn->studentNumber)
        {
            printf("도서 번호 : %08d\n", tempHead->bookNumber);

            unix_timestamp1 = tempHead->borrowTime;
            utc_timestamp_to_date(unix_timestamp1, &datetime1);
            printf("대여일짜 : %d년 %d월 %d일 %d시 %d 분\n", datetime1.year, datetime1.month, datetime1.day, datetime1.hour, datetime1.minute);

            unix_timestamp2 = tempHead->returnTime;
            utc_timestamp_to_date(unix_timestamp2, &datetime1);
            printf("반납일짜 : %d년 %d월 %d일 %d시 %d 분\n", datetime2.year, datetime2.month, datetime2.day, datetime2.hour, datetime2.minute);
            break;
        }
        else
        {
            tempHead = tempHead->nextNode;
        }
    }
}

// 개인정보를 업데이트하는 함수입니다.
void updateMyProfile()
{
    int selectNum;
    char tempString[100];

    printf("1.비밀번호\t2.주소\t3.전화번호\nselect : ");
    scanf("%d", &selectNum);

    if (selectNum == 1)
    {
        printf("새 비밀번호 입력 : ");
        scanf("%s", tempString);
        strcpy(clientLoggedIn->password, tempString);
        printf("비밀번호 변경완료\n");
        return;
    }

    else if (selectNum == 1)
    {
        printf("새 주소 입력 : ");
        scanf("%s", tempString);
        strcpy(clientLoggedIn->address, tempString);
        printf("주소 변경완료\n");
        return;
    }

    else if (selectNum == 3)
    {
        printf("새 전화번호 입력 : ");
        scanf("%s", tempString);
        strcpy(clientLoggedIn->address, tempString);
        printf("전화번호 변경완료\n");
        return;
    }

    else
    {
        ;
    }
}

// 회원 탈퇴를 담당하는 함수입니다.
void clientWithdraw()
{
    borrowStruct *tempHead = borrowHeadNode;
    clientStruct *tempClientHead = clientHeadNode;

    while (tempHead->nextNode != NULL)
    {
        if (tempHead->studentNumber == clientLoggedIn->studentNumber)
        {
            printf("대여중인 도서가 있으므로 회원탈퇴가 불가합니다.");
            return;
        }
        else
        {
            tempHead = tempHead->nextNode;
        }
    }
    while (tempClientHead != clientLoggedIn)
    {
        if (tempClientHead->nextNode == clientLoggedIn)
        {
            tempClientHead->nextNode = clientLoggedIn->nextNode;
            free(clientLoggedIn);
            printf("회원 탈퇴가 완료되었습니다.");
            main();
        }

        else
        {
            tempClientHead = tempClientHead->nextNode;
        }
    }
}

// 회원 목록 출력을 담당하는 함수입니다.
void printClientList()
{
    int i = 1;
    clientStruct *tempClientHead = clientHeadNode;
    printf(">>회원목록<<\n");
    while (tempClientHead->nextNode != NULL)
    {
        printf("%d. %s ( 학번 : %d )\n", i, tempClientHead->name, tempClientHead->studentNumber);
        i++;
        tempClientHead = tempClientHead->nextNode;
    }
}

// 도서를 등록하는 함수입니다.
// 수정 부분 없습니다. 완료된 함수입니다.
void registerBook()
{
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
    bookNumber = booktail->bookNumber + 1;
    printf("대여가능 여부 : %c\n", canBorrow);
    printf("도서번호: %d\n\n", bookNumber);

    char check;
    printf("등록하시겠습니까? ");
    scanf("%c", &check);

    bookStruct *newbook = (bookStruct *)malloc(sizeof(bookStruct));
    bookStruct *StpFront = bookHeadNode;
    bookStruct *StpRear = bookHeadNode;
    if (check == 'Y')
    {
        newbook->ISBN = ISBN;
        newbook->bookNumber = bookNumber;
        newbook->canBorrow = canBorrow;
        saveStringByDynamicAlloc(&(newbook->bookName), bookName);
        saveStringByDynamicAlloc(&(newbook->publisher), publisher);
        saveStringByDynamicAlloc(&(newbook->author), author);
        saveStringByDynamicAlloc(&(newbook->location), location);
        newbook->nextNode = NULL;

        while (0 != StpRear) //	 삽입 위치  검색 	 //중간 삽입 코드
        {
            if (StpRear->ISBN > newbook->ISBN) // 삽입 위치 판단
            {
                break; // insert location !
            }
            StpFront = StpRear;          //Front = Rear ㅇㅇ
            StpRear = StpRear->nextNode; //Rear 가 한칸 이동!
        }
        if (bookHeadNode != StpRear) //중간 혹은 끝부분 삽입 시
        {
            newbook->nextNode = StpRear;
            StpFront->nextNode = newbook;
        }
        else //앞 삽입

        {
            newbook->nextNode = bookHeadNode;
            bookHeadNode = newbook;
        }
    }
    printf("newbook : %d\n", (newbook->nextNode)->ISBN);
    printf("\n등록이 완료되었습니다.\n");
    bookStruct *tempBookHead = bookHeadNode;
}

// 도서를 삭제하는 함수입니다.
void removeBook()
{
    int num, ISBN, bookdeletechecknum = 0, booknum;
    char bookName[50];
    char bookdeletecheck;
    bookStruct *bookSeeker = (bookStruct *)(malloc(sizeof(bookStruct)));
    bookSeeker = bookHeadNode;

    printf(">>도서 삭제<<\n\n");
    printf("1. 도서명 검색 2. ISBN 검색\n\n");
    printf("검색 번호를 입력하세요 : ");
    scanf("%d", &num);
    switch (num)
    {
    case 1:
        printf("\n도서명을 입력하세요 : ");
        scanf("%s", bookName);
        // printf("bookname : %s", bookName);
        while (booktail != bookSeeker)
        {
            if (strcmp(bookSeeker->bookName, bookName) == 0)
            {
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
            if (bookSeeker->ISBN == ISBN)
            {
                printf("도서번호 : %d(삭제 가능 여부 : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("도서명 : %s\n", bookSeeker->bookName);
                printf("출판사 : %s\n", bookSeeker->publisher);
                printf("저자명 : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("소장처 : %s\n", bookSeeker->location);
                if (bookSeeker->canBorrow == 'Y')
                {
                    bookdeletechecknum++;
                }
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    }
    if (bookdeletechecknum > 0)
    {
        printf("\n삭제할 도서번호를 입력하세요 : ");
        scanf("%d", &booknum);
        getchar();
        printf("\n\n이 도서를 삭제합니까? (Y/N) : ");
        scanf("%c", &bookdeletecheck);
        if (bookdeletecheck == 'Y')
        {
            //도서삭제코드
            bookStruct *StpFront = bookHeadNode;
            bookStruct *StpRear = bookHeadNode;
            while (0 != StpRear)
            {
                //Rear는 삭제될 노드, front는 삭제될 노드의 앞 노드를 가르킴
                //front를 이용하여 rear삭제 예정
                if (StpRear->bookNumber == booknum) // 삭제 노드 위치(head, 중간) 판단
                {
                    break; // 삭제할 노드 찾을 때 까지 반복
                }
                StpFront = StpRear;          //Front = Rear
                StpRear = StpRear->nextNode; //Rear 가 한칸 이동
            }
            if (bookHeadNode != StpRear) //중간노드 삭제
            {
                StpFront = StpRear->nextNode; //front가 삭제할 노드의 다음을 가르킵니다.

                bookHeadNode->nextNode = StpFront;
            }
            else if (bookHeadNode == StpRear) // Head -> booknum 을 삭제
            {
                bookHeadNode = bookHeadNode->nextNode; //head가 다음 노드 가르킴
                free(StpRear);
            }
            else if (0 == StpRear)
            {
                printf("잘못된 입력입니다\n");
            }
            printf("\n\n도서가 삭제 되었습니다.\n\n ");
        }
    }
}

// 도서를 대여하는 함수입니다.
void rentalBook()
{
    int num, ISBN, studentnum, booknum;
    char bookName[50];
    char checkUserBorrow;
    bookStruct *bookSeeker = (bookStruct *)(malloc(sizeof(bookStruct)));
    bookSeeker = bookHeadNode;
    printf(">>도서 대여<<\n\n");
    printf("1. 도서명 검색 2. ISBN 검색\n\n");
    printf("검색 번호를 입력하세요 : ");

    scanf("%d", &num);

    switch (num)
    {
    case 1:
        printf("\n도서명을 입력하세요 : ");
        scanf("%s", bookName);
        while (booktail != bookSeeker)
        {
            if (strcmp(bookSeeker->bookName, bookName) == 0)
            {
                // 도서 이름으로 검색 성공하면.
                printf("\n>>검색 결과<<\n");
                printf("도서번호 : %d (대여 가능 여부 : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("도서명 : %s\n", bookSeeker->bookName);
                printf("출판사 : %s\n", bookSeeker->publisher);
                printf("저자명 : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("소장처 : %s\n", bookSeeker->location);
                if (bookSeeker->canBorrow == 'Y')
                {
                    printf("이 도서를 대여하시겠습니까? (Y/N)");
                    scanf("%c", &checkUserBorrow);
                    if (checkUserBorrow == 'Y')
                    {
                        bookSeeker->canBorrow = 'N';
                    }
                }
                else
                {
                    printf("이 도서를 대여할 수 없습니다.\n\n");
                }
                break;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;

    case 2:
        printf("\nISBN을 입력하세요 : ");
        scanf("%d", &ISBN);
        while (0 != bookSeeker)
        {
            if (bookSeeker->ISBN == ISBN)
            {
                printf("도서번호 : %d(대여 가능 여부 : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("도서명 : %s\n", bookSeeker->bookName);
                printf("출판사 : %s\n", bookSeeker->publisher);
                printf("저자명 : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("소장처 : %s\n", bookSeeker->location);
                if (bookSeeker->canBorrow == 'Y')
                {
                    printf("이 도서를 대여하시겠습니까? (Y/N)");
                    scanf("%c", &checkUserBorrow);
                    if (checkUserBorrow == 'Y')
                    {
                        bookSeeker->canBorrow = 'N';
                    }
                }
                else
                {
                    printf("이 도서를 대여할 수 없습니다.\n\n");
                }
                break;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    }
}

// 도서를 반납하는 함수입니다.
// 세연님이 구현한 함수입니다.
void returnBook()
{
    int studentnum, booknum, bookreturnchecknum = 0;
    char bookreturncheck;
    bookStruct *bookSeeker = (bookStruct *)(malloc(sizeof(bookStruct)));
    bookSeeker = bookHeadNode;

    // showMyRentalList() 함수 참고하였습니다.
    timestamp_t unix_timestamp1;
    timestamp_t unix_timestamp2;
    datetime_t datetime1;
    datetime_t datetime2;

    borrowStruct *tempHead = borrowHeadNode;

    printf("\n학번을 입력하세요 : ");
    scanf("%d", &studentnum);

    printf("\n\n>> 회원의 대여 목록 <<"); // showMyRentalList() 함수 참고하였습니다.
    while (tempHead->nextNode != NULL)
    {
        if (tempHead->studentNumber == clientLoggedIn->studentNumber)
        {
            printf("도서 번호 : %08d\n", tempHead->bookNumber);

            unix_timestamp1 = tempHead->borrowTime;
            utc_timestamp_to_date(unix_timestamp1, &datetime1);
            printf("대여 일자 : %d년 %d월 %d일 %d시 %d 분\n", datetime1.year, datetime1.month, datetime1.day, datetime1.hour, datetime1.minute);

            unix_timestamp2 = tempHead->returnTime;
            utc_timestamp_to_date(unix_timestamp2, &datetime1);
            printf("반납 일자 : %d년 %d월 %d일 %d시 %d 분\n", datetime2.year, datetime2.month, datetime2.day, datetime2.hour, datetime2.minute);
            break;
        }
        else
        {
            tempHead = tempHead->nextNode;
        }
    }

    printf("\n\n 반납할 도서번호를 입력하세요 : ");
    scanf("%d", &booknum);

    printf("도서 반납처리를 할까요?");
    scanf("%c", &bookreturncheck);

    if (bookreturncheck == 'N')
    {
        while (1)
        {
            // BOOKNUM이 일치하는 노드를 찾을 때 까지 반복
            if (bookHeadNode->bookNumber == booknum)
            {
                bookHeadNode->canBorrow = 'Y'; // 책번호가 일치하면 도서 반납 완료 & 대여 가능 여부 N으로 바꾸기
                break;
            }
            bookHeadNode = bookHeadNode->nextNode;
        }
        printf("\n\n도서가 반납 되었습니다.\n\n ");
    }
}

// 도서를 검색하는 함수입니다 - byName
void searchBookByName()
{
    char targetBookName[100];
    short found = 0;
    bookNode = bookHeadNode;
    printf("도서 제목을 입력하세요: ");
    scanf("%s", targetBookName);
    while (bookNode->nextNode != NULL)
    {
        if (strcmp(bookNode->bookName, targetBookName))
        {
            printf("\n도서를 찾았습니다.\n");
            printf("도서번호 : %d (대여 가능 여부 : %c) \n", bookNode->bookNumber, bookNode->canBorrow);
            printf("도서명 : %s\n", bookNode->bookName);
            printf("출판사 : %s\n", bookNode->publisher);
            printf("저자명 : %s\n", bookNode->author);
            printf("ISBN : %d\n", bookNode->ISBN);
            printf("소장처 : %s\n", bookNode->location);
            found = 1;
            break;
        }
        bookNode = bookNode->nextNode;
    }
    if (found == 0)
    {
        printf("\n도서를 찾지못했습니다.\n");
    }
}

// 도서를 검색하는 함수입니다 - byPublisher
void searchBookByPublisher()
{
    char targetBookPublisher[100];
    short found = 0;
    bookNode = bookHeadNode;
    printf("도서 출판사를 입력하세요: ");
    scanf("%s", targetBookPublisher);
    while (bookNode->nextNode != NULL)
    {
        if (strcmp(bookNode->publisher, targetBookPublisher))
        {
            printf("\n도서를 찾았습니다.\n");
            printf("도서번호 : %d (대여 가능 여부 : %c) \n", bookNode->bookNumber, bookNode->canBorrow);
            printf("도서명 : %s\n", bookNode->bookName);
            printf("출판사 : %s\n", bookNode->publisher);
            printf("저자명 : %s\n", bookNode->author);
            printf("ISBN : %d\n", bookNode->ISBN);
            printf("소장처 : %s\n", bookNode->location);
            found = 1;
            break;
        }
        bookNode = bookNode->nextNode;
    }
    if (found == 0)
    {
        printf("\n도서를 찾지못했습니다.\n");
    }
}

// 도서를 검색하는 함수입니다 - byISBN
void searchBookByISBN()
{
    int targetBookISBN;
    short found = 0;
    bookNode = bookHeadNode;
    printf("도서 출판사를 입력하세요: ");
    scanf("%d", &targetBookISBN);
    while (bookNode->nextNode != NULL)
    {
        if (bookNode->ISBN == targetBookISBN)
        {
            printf("\n도서를 찾았습니다.\n");
            printf("도서번호 : %d (대여 가능 여부 : %c) \n", bookNode->bookNumber, bookNode->canBorrow);
            printf("도서명 : %s\n", bookNode->bookName);
            printf("출판사 : %s\n", bookNode->publisher);
            printf("저자명 : %s\n", bookNode->author);
            printf("ISBN : %d\n", bookNode->ISBN);
            printf("소장처 : %s\n", bookNode->location);
            found = 1;
            break;
        }
        bookNode = bookNode->nextNode;
    }
    if (found == 0)
    {
        printf("\n도서를 찾지못했습니다.\n");
    }
}

// 도서를 검색하는 함수입니다 - byAuthor
void searchBookByAuthor()
{
    char targetBookAuthor[100];
    short found = 0;
    bookNode = bookHeadNode;
    printf("도서 출판사를 입력하세요: ");
    scanf("%s", targetBookAuthor);
    while (bookNode->nextNode != NULL)
    {
        if (strcmp(bookNode->author, targetBookAuthor))
        {
            printf("\n도서를 찾았습니다.\n");
            printf("도서번호 : %d (대여 가능 여부 : %c) \n", bookNode->bookNumber, bookNode->canBorrow);
            printf("도서명 : %s\n", bookNode->bookName);
            printf("출판사 : %s\n", bookNode->publisher);
            printf("저자명 : %s\n", bookNode->author);
            printf("ISBN : %d\n", bookNode->ISBN);
            printf("소장처 : %s\n", bookNode->location);
            found = 1;
            break;
        }
        bookNode = bookNode->nextNode;
    }
    if (found == 0)
    {
        printf("\n도서를 찾지못했습니다.\n");
    }
}

// 도서를 검색하는 함수입니다 - byAll
void searchBookByAll()
{
    bookNode = bookHeadNode;
    while (bookNode->nextNode != NULL)
    {
        printf("\n\n도서를 찾았습니다.\n");
        printf("도서번호 : %d (대여 가능 여부 : %c) \n", bookNode->bookNumber, bookNode->canBorrow);
        printf("도서명 : %s\n", bookNode->bookName);
        printf("출판사 : %s\n", bookNode->publisher);
        printf("저자명 : %s\n", bookNode->author);
        printf("ISBN : %d\n", bookNode->ISBN);
        printf("소장처 : %s\n", bookNode->location);
        bookNode = bookNode->nextNode;
    }
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

// 타임 스탬프를 기준으로 요일 얻기
uint8_t timestamp_to_weekday(timestamp_t timestamp_sec)
{
    uint8_t result = (timestamp_sec / ONE_DAY + UTC_TIME_WEEKDAY_OFFSET) % 7;
    if (result == 0)
    {
        result = 7;
    }
    return result;
}

// 윤달 확인
int is_leap_year(uint16_t year)
{
    if (year % 4 == 0 && ((year % 100) != 0) || ((year % 400) == 0))
    {
        return 1;
    }
    else
        return 0;
}

// utc 타임 스탬프를 날짜로 변환
void utc_timestamp_to_date(timestamp_t timestamp, datetime_t *datetime)
{
    uint8_t month;
    uint32_t days;
    uint16_t days_in_year;
    uint16_t year;
    timestamp_t second_in_day;

    // 시/분/초 계산
    second_in_day = timestamp % ONE_DAY;

    //초
    datetime->second = second_in_day % 60;

    //분
    second_in_day /= 60;
    datetime->minute = second_in_day % 60;

    //시
    second_in_day /= 60;
    datetime->hour = second_in_day % 24;

    //1970-1-1 0:0:0부터 현재까지 총 일수
    days = timestamp / ONE_DAY;

    //days를 계속 차감하면서 해당 년도 계산
    for (year = 1970; year <= 2200; year++)
    {
        if (is_leap_year(year))
            days_in_year = 366;
        else
            days_in_year = 365;

        if (days >= days_in_year)
            days -= days_in_year;
        else
            break;
    }

    //년
    datetime->year = year;

    //요일
    datetime->weekday = timestamp_to_weekday(timestamp);

    //해당 년도 1월 1일을 기준으로 지금까지의 주(week) 계산
    datetime->week = (days + 11 - datetime->weekday) / 7;

    //월 계산하기
    if (is_leap_year(datetime->year)) //윤달의 경우 2월이 29일이다.
        month_days[2] = 29;
    else
        month_days[2] = 28;

    //년도와 마찬가지로 일에서 계속 차감해서 찾는다.
    for (month = 1; month <= 12; month++)
    {
        if (days >= month_days[month])
            days -= month_days[month];
        else
            break;
    }
    datetime->month = month;
    datetime->day = days + 1;
}

// 개발용 로그를 출력하는 함수입니다.
// 자동 줄바꿈 처리가 되므로, 개행문자는 입력하지 않는게 좋습니다.
// 프로젝트 마무리 후 모든 관련 항목들을 제거해야합니다.
void printLog(char *message)
{
    printf("\n[LOG] %s\n", message);
}

// 파일들을 저장 후, 종료하는 함수입니다.
void saveAndExit()
{
    saveFileToStruct_Client();
    saveFileToStruct_Book();
    saveFileToStruct_Borrow();
    exit(1);
}