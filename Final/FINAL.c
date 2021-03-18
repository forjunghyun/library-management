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

// TODO: struct를 txt로 저장하는 함수들 전부 - 영현

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
            exit(1);
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
            exit(1);
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
            exit(1);
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
        printf("5. 전체 검색\n");
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

// 로그인을 담당하는 함수입니다.
void login()
{
    while (1)
    {
        printLog("로그인에 대한 기능을 구현해야합니다.");
        // TODO: 로그인 기능 구현 / 서현님
        printLog("1을 입력하면 회원메뉴, 2를 입력하면 관리자 메뉴로 갑니다.");

        /* 일반 회원, 관리자 메뉴에서 "로그아웃"을 하면 다시 이 곳으로 돌아옵니다.
"로그아웃"은 return으로 구현됩니다. 따라서 이 곳에서 while(1)문으로 처리해줘야합니다. */

        int a;
        scanf("%d", &a);
        if (a == 1)
        {
            menuPresenter_NormalClient();
        }
        else
        {
            menuPresenter_Administrator();
        }
    }
}

// 내 도서 대여 목록을 보여주는 함수입니다.
void showMyRentalList()
{
    printLog("내 도서 대여 목록을 보여줘야합니다.");
    // TODO: 서현님
}

// 개인정보를 업데이트하는 함수입니다.
void updateMyProfile()
{
    printLog("내 개인정보 수정을 구현해야 합니다.");
    // TODO: 서현님
}

// 회원 탈퇴를 담당하는 함수입니다.
void clientWithdraw()
{
    printLog("회원 탈퇴를 구현해야합니다.");
    // TODO: 서현님
}

// 회원 목록 출력을 담당하는 함수입니다.
void printClientList()
{
    printLog("회원 목록 출력을 구현해야합니다.");
    // TODO: 서현님
}

// 도서를 등록하는 함수입니다.
void registerBook()
{
    printLog("도서 등록을 구현해야합니다.");
    // TODO: 민비 & 세연님
}

// 도서를 삭제하는 함수입니다.
void removeBook()
{
    printLog("도서 삭제를 구현해야합니다.");
    // TODO: 민비 & 세연님
}

// 도서를 대여하는 함수입니다.
void rentalBook()
{
    printLog("도서 대여를 구현해야합니다.");
    // TODO: 민비 & 세연님
}

// 도서를 반납하는 함수입니다.
void returnBook()
{
    printLog("도서 반납을 구현해야합니다.");
    // TODO: 민비 & 세연님
}

// 도서를 검색하는 함수입니다 - byName
void searchBookByName()
{
    printLog("도서 검색을 구현해야합니다. byName");
    // TODO: 영현님
}

// 도서를 검색하는 함수입니다 - byPublisher
void searchBookByPublisher()
{
    printLog("도서 검색을 구현해야합니다. byPublisher");
    // TODO: 영현님
}

// 도서를 검색하는 함수입니다 - byISBN
void searchBookByISBN()
{
    printLog("도서 검색을 구현해야합니다. byISBN");
    // TODO: 영현님
}

// 도서를 검색하는 함수입니다 - byAuthor
void searchBookByAuthor()
{
    printLog("도서 검색을 구현해야합니다. byAuthor");
    // TODO: 영현님
}

// 도서를 검색하는 함수입니다 - byAll
void searchBookByAll()
{
    printLog("도서 검색을 구현해야합니다. byAll");
    // TODO: 영현님
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