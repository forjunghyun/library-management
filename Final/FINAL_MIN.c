#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)


// Struct Type���� �����ϴ� �κ��Դϴ�.
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

// ���� Node �������� �����ϴ� �κ��Դϴ�.
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

// �Լ����� ��� ������Դϴ�.
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

// ���α׷� ���� ���� �κ��Դϴ�.
int main(void)
{
    // ���� �ҷ�������� �����մϴ�.
    readFileToStruct_Book();
    removeBook();
    //registerBook();
    return 0;
}


// book.txt�� bookStruct�� �����ϴ� �Լ��Դϴ�.
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
// book.txt�� bookStruct�� �����ϴ� �Լ��Դϴ�.

// ������ ����ϴ� �Լ��Դϴ�.
void registerBook()
{
    printLog("���� ����� �����ؾ��մϴ�.");
    int bookNumber;
    char bookName[50];
    char publisher[50];
    char author[50];
    int ISBN;
    char location[50];
    char canBorrow;

    printf("������ : ");
    scanf("%s", bookName);
    printf("���ǻ� : ");
    scanf("%s", publisher);
    printf("���ڸ� : ");
    scanf("%s", author);
    printf("ISBN : ");
    scanf("%d", &ISBN);
    printf("����ó : ");
    scanf("%s", location);
    getchar();

    printf("\n�ڵ��Է� ����\n\n");
    canBorrow = 'Y';
    bookNumber = booktail->bookNumber +1;
    printf("�뿩���� ���� : %c\n",canBorrow);
    printf("������ȣ: %d\n\n", bookNumber);

    char check;
    printf("����Ͻðڽ��ϱ�? ");
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

        while (0 != StpRear)	//	 ���� ��ġ  �˻� 	 //�߰� ���� �ڵ�
        {
            if (StpRear->ISBN > newbook->ISBN)	// ���� ��ġ �Ǵ� 
            {
                break;	// insert location ! 
            }
            StpFront = StpRear;				 //Front = Rear ����
            StpRear = StpRear->nextNode;			 //Rear �� ��ĭ �̵�!
        }
        if (bookHeadNode != StpRear)			 //�߰� Ȥ�� ���κ� ���� ��
        {
            newbook->nextNode = StpRear;
            StpFront->nextNode = newbook;
        }
        else			 //�� ����

        {
            newbook->nextNode = bookHeadNode;
            bookHeadNode = newbook;
        }
    }
    printf("newbook : %d\n", newbook->ISBN);
    printf("\n����� �Ϸ�Ǿ����ϴ�.\n");
    bookStruct* tempBookHead = bookHeadNode;

            //TODO:�߰��� ���� ��� �ȵ�
            printf(">>�������<<\n");
            while (tempBookHead != NULL)
            {
                printf("%d. %s \n", tempBookHead->ISBN, tempBookHead->bookName);
                tempBookHead = tempBookHead->nextNode;
            }
}

// ������ �����ϴ� �Լ��Դϴ�.
void removeBook()
{
    int num,ISBN,bookdeletechecknum=0,booknum;
    char bookName[50];
    char bookdeletecheck;
    bookStruct* bookSeeker = (bookStruct*)(malloc(sizeof(bookStruct)));
    bookSeeker=bookHeadNode;
    printf("bookseeker bookname : %s\n", bookSeeker->bookName);

    printLog(">>���� ����<<\n\n");
    printf("1.������ �˻� 2. ISBN �˻�\n\n");
    printf("�˻� ��ȣ�� �Է��ϼ��� : ");
    scanf("%d", &num);
    switch (num)
    {
    case 1:
        printf("\n�������� �Է��ϼ��� : ");
        scanf("%s", bookName);
        printf("bookname : %s", bookName);
        while (booktail != bookSeeker)
        {
            if (strcmp(bookSeeker->bookName, bookName)==0) {
                printf("������ȣ : %d(���� ���� ���� : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("������ : %s\n", bookSeeker->bookName);
                printf("���ǻ� : %s\n", bookSeeker->publisher);
                printf("���ڸ� : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("����ó : %s\n", bookSeeker->location);
                bookdeletechecknum++;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    case 2:
        printf("\nISBN�� �Է��ϼ��� : ");
        scanf("%d", &ISBN);
        while (0 != bookSeeker)
        {
            if (bookSeeker->ISBN== ISBN) {
                printf("������ȣ : %d(���� ���� ���� : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("������ : %s\n", bookSeeker->bookName);
                printf("���ǻ� : %s\n", bookSeeker->publisher);
                printf("���ڸ� : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("����ó : %s\n", bookSeeker->location);
                if (bookSeeker->canBorrow == 'Y') {
                    bookdeletechecknum++;
                }
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    }
    if (bookdeletechecknum > 0) {
        printf("\n������ ������ȣ�� �Է��ϼ��� : ");
        scanf("%d", &booknum);
        getchar();
        printf("\n\n�� ������ �����մϱ�? : ");
        scanf("%c", &bookdeletecheck);
        if (bookdeletecheck == 'Y') {
            //���������ڵ�
            bookStruct* StpFront = bookHeadNode;
            bookStruct* StpRear = bookHeadNode;
            while (0 != StpRear)	//	 ���� ��ġ  �˻� 	 //�߰� ���� �ڵ�
            {
                if (StpRear->bookNumber== booknum)	// ���� ��ġ �Ǵ� 
                {
                    break;	// insert location ! 
                }
                StpFront = StpRear;				 //Front = Rear
                StpRear = StpRear->nextNode;			 //Rear �� ��ĭ �̵�
            }
            if (bookHeadNode != StpRear)			 //�߰� Ȥ�� ���κ� ���� ��
            {

                //TODO: �߰��κ� ���� �ڵ�

            }
            else if (bookHeadNode == StpRear)	 // Head -> booknum �� ���� ��
            {
                bookHeadNode = bookHeadNode->nextNode;
                free(StpRear);
            }
            else if (0 == StpRear)
            {
                printf("�߸��� �Է��Դϴ�\n");
            }
            printf("\n\n������ ���� �Ǿ����ϴ�.\n\n ");
        }
     }
    bookStruct* tempBookHead = bookHeadNode;
    printf(">>�������<<\n");
    while (tempBookHead != NULL)
    {
        printf("%d. %s \n", tempBookHead->ISBN, tempBookHead->bookName);
        tempBookHead = tempBookHead->nextNode;
    }
}

// ������ �뿩�ϴ� �Լ��Դϴ�.
void rentalBook()
{
    int num,ISBN,studentnum, booknum,bookborrowchecknum=0;
    char bookName[50];
    char bookborrowcheck;
    bookStruct* bookSeeker = (bookStruct*)(malloc(sizeof(bookStruct)));
    bookSeeker = bookHeadNode;
    printLog(">>���� �뿩<<\n\n");
    printf("1.������ �˻� 2. ISBN �˻�\n\n");
    printf("�˻� ��ȣ�� �Է��ϼ��� : ");
    scanf("%d", &num);
    switch (num)
    {
    case 1:
        printf("\n�������� �Է��ϼ��� : ");
        scanf("%s", bookName);
        printf("bookname : %s", bookName);
        while (booktail != bookSeeker)
        {
            if (strcmp(bookSeeker->bookName, bookName) == 0) {
                printf(">>�˻����<<\n");
                printf("������ȣ : %d(�뿩 ���� ���� : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("������ : %s\n", bookSeeker->bookName);
                printf("���ǻ� : %s\n", bookSeeker->publisher);
                printf("���ڸ� : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("����ó : %s\n", bookSeeker->location);
                bookborrowchecknum++;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    case 2:
        printf("\nISBN�� �Է��ϼ��� : ");
        scanf("%d", &ISBN);
        while (0 != bookSeeker)
        {
            if (bookSeeker->ISBN == ISBN) {
                printf("������ȣ : %d(�뿩 ���� ���� : %c) \n", bookSeeker->bookNumber, bookSeeker->canBorrow);
                printf("������ : %s\n", bookSeeker->bookName);
                printf("���ǻ� : %s\n", bookSeeker->publisher);
                printf("���ڸ� : %s\n", bookSeeker->author);
                printf("ISBN : %d\n", bookSeeker->ISBN);
                printf("����ó : %s\n", bookSeeker->location);
                bookborrowchecknum++;
            }
            bookSeeker = bookSeeker->nextNode;
        }
        break;
    }
    if (bookborrowchecknum > 0) {
        printf("\n�й��� �Է��ϼ��� : ");
        scanf("%d", &studentnum);
        printf("\n������ȣ�� �Է��ϼ��� : ");
        scanf("%d", &booknum);
        getchar();
        printf("\n\n�� ������ �뿩�մϱ�? : ");
        scanf("%c", &bookborrowcheck);
        if (bookborrowcheck == 'Y') {
            //TODO : �л��� ���� �뿩 ���� ����
            printf("\n\n������ �뿩 �Ǿ����ϴ�.\n\n ");
        }
    }
    // TODO: �κ� & ������
}

// ������ �ݳ��ϴ� �Լ��Դϴ�.
void returnBook()
{
    printLog("���� �ݳ��� �����ؾ��մϴ�.");
    // TODO: �κ� & ������
}
// �Է¹��� string�� ���� �Ҵ����� �����ϴ� �Լ��Դϴ�.
void saveStringByDynamicAlloc(char** destPtr, char* sourcePtr)
{
    *destPtr = (char*)malloc(strlen(sourcePtr) + 1);
    strcpy(*destPtr, sourcePtr);
}

// ���߿� �α׸� ����ϴ� �Լ��Դϴ�.
// �ڵ� �ٹٲ� ó���� �ǹǷ�, ���๮�ڴ� �Է����� �ʴ°� �����ϴ�.
// ������Ʈ ������ �� ��� ���� �׸���� �����ؾ��մϴ�.
void printLog(char** message)
{
    printf("\n[LOG] %s\n", message);
}