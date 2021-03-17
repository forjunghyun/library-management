#pragma once
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#pragma warning(disable  : 4996)
#include"LibraryMagagement.h"



//파일읽기 프로그램--------------------------------------------------------------------------------------------
clientStruct* clientHeadNode;
clientStruct* clienttail;
bookStruct* bookHeadNode;
bookStruct* booktail;
Borrow* BorrowHeadNode;
Borrow* Borrowtail;



void file_read()
{
	FILE* client_file, * book_file, * borrow_file;
	//client 시작
	char client_tempString[999];
	char* client_tempTokPtr;

	clientHeadNode = (clientStruct*)(malloc(sizeof(clientStruct)));
	clienttail = (clientStruct*)(malloc(sizeof(clientStruct)));
	clientStruct* clientNode = (clientStruct*)(malloc(sizeof(clientStruct)));
	clientStruct* tempClientNode;


	clientHeadNode = clientNode;


	client_file = fopen("client.txt", "r");

	while ((fscanf(client_file, "%[^\n]", client_tempString)) != EOF) {
		clientNode->studentNumber = atoi(strtok(client_tempString, "|"));
		saveStringByDynamicAlloc(&(clientNode->password), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->name), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->address), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->phoneNumber), strtok(NULL, "|"));

		tempClientNode = (clientStruct*)(malloc(sizeof(clientStruct)));

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
	char* book_tempTokPtr;

	// node to save first node.
	bookHeadNode = (bookStruct*)(malloc(sizeof(bookStruct)));
	booktail = (bookStruct*)(malloc(sizeof(bookStruct)));

	// dynamic alloc for normal node
	bookStruct* bookNode = (bookStruct*)(malloc(sizeof(bookStruct)));
	bookStruct* tempBookNode;

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
		tempBookNode = (bookStruct*)(malloc(sizeof(bookStruct)));

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

	//borrow 시작
	borrow_file = fopen("borrow.txt", "r");
	char borrow_tempString[999];
	char* borrow_tempTokPtr;

	BorrowHeadNode = (Borrow*)malloc(sizeof(Borrow));
	Borrowtail = (Borrow*)(malloc(sizeof(Borrow)));
	Borrow* BorrowNode = (Borrow*)malloc(sizeof(Borrow));
	Borrow* tempBorrowNode;

	BorrowHeadNode = BorrowNode;

	char* token;


	while (1) {
		fscanf(borrow_file, "%s", borrow_tempString);
		if (feof(borrow_file)) {
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

		tempBorrowNode = (Borrow*)malloc(sizeof(Borrow));

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

void saveStringByDynamicAlloc(char** destPtr, char* sourcePtr)
{
	*destPtr = (char*)malloc(strlen(sourcePtr) + 1);
	strcpy(*destPtr, sourcePtr);
}
void linkedlist_store()
{

	return;
}






//회원용 프로그램--------------------------------------------------------------------------------------------
void memberRegister(clientStruct* clientHeadNode); //회원등록
void loginMember(clientStruct* clientHeadNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode); //로그인메뉴
void loginScreen(clientStruct* clientNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode);//로그인한 후 회원메뉴창
void searchBook(clientStruct* clientNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode); //도서검색
void myBorrowList(clientStruct* clientNode, Borrow* borrowHeadNode); //내 대여목록
void modifyInfo(clientStruct* clientNode);//개인정보수정
void eraseMem(clientStruct* clientNode, Borrow* borrowHeadNode);//회원탈퇴
//도서검색 메뉴----------------------------------------------------------------------------------------------
void searchBookName(bookStruct* bookheadNode);
void searchAuthor(bookStruct* bookheadNode);
void searchISBN(bookStruct* bookheadNode);
void searchPublisher(bookStruct* bookheadNode);
void searchAll(bookStruct* bookheadNode);
//------------------------------------------------------------------------------------------------------------







int main() {
	file_read();

	int selectService;

	


	while (1) {
		printf(">>도서관 서비스<<\n");
		printf("1.회원가입\t2.로그인\t2.프로그램종료\select : ");
		scanf("%d", &selectService);

		if (selectService == 1) {
			memberRegister(clientHeadNode);
		}

		else if (selectService == 2) {
			loginMember(clientHeadNode, bookHeadNode, BorrowHeadNode);
		}

		else if (selectService == 3) {
			return 0;
		}

		else {
			;
		}

	}
	
}

void memberRegister(clientStruct* clientHeadNode) {
	clientStruct* temphead;
	clientStruct* newMember = (clientStruct*)malloc(sizeof(clientStruct));
	int true = 1;
	int tempInt;
	char tempString[100];

	printf(">>회원가입<<\n");

	while (true) {
		printf("학번 입력 : ");
		scanf("%d", &tempInt);

		temphead = clientHeadNode;
		while (temphead->nextNode != NULL) {

			if (temphead->studentNumber == tempInt) {
				printf("이미 등록되어있는 회원입니다.\n");
				break;
			}
			else {
				temphead = temphead->nextNode;
			}

			
		}

		if (temphead->nextNode == NULL && temphead->studentNumber != tempInt) {
			true = 0;
			newMember->studentNumber = tempInt;
		}
		else { ; }

	}

	printf("비밀 번호 입력 : ");
	scanf("&s", tempString);
	strcpy(newMember->password, tempString);

	printf("이름 입력 : ");
	scanf("%s", tempString);
	strcpy(newMember->name, tempString);

	printf("주소 입력 : ");
	strcpy(newMember->address, tempString);

	printf("전화 번호 입력 : ");
	strcpy(newMember->phoneNumber, tempString);

	temphead = clientHeadNode;

	while (temphead->nextNode != NULL) { temphead = temphead->nextNode; }

	temphead->nextNode = newMember;
	newMember->nextNode = NULL;

	free(temphead);


}

void loginMember(clientStruct* clientHeadNode,bookStruct* bookHeadNode,Borrow* borrowHeadNode) {
	int tempInt;
	clientStruct* temphead;
	char tempString[50];
	int true = 1;
	
	printf(">>로그인 화면<<\n");
	while (true){
		temphead = clientHeadNode;
		printf("로그인명(학번) : ");
		scanf("%d", &tempInt);
		printf("비민번호 입력 : ");
		scanf("%s", tempString);

		while (temphead->nextNode != NULL) {
			if (temphead->studentNumber == tempInt && strcmp(temphead->password, tempString) == 0) {
				loginScreen(temphead,bookHeadNode,borrowHeadNode);
				true = 0;
			}
			else {
				temphead = temphead->nextNode;
			}
		}

		if (temphead->nextNode == NULL) {
			printf("없는 회원입니다.");
		}
	}

}

void loginScreen(clientStruct* clientNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode) {
	int selectMenu;

	while (1) {
		printf(">>회원 메뉴<<\n");
		printf("1.도서검색\t2.내 대여목록\n3.개인정보 수정\t4.회원탈퇴\n5.로그아웃\t6.프로그램종료\nselect : ");
		scanf("%d", &selectMenu);

		if (selectMenu == 1) {
			searchBook(clientNode, bookHeadNode, borrowHeadNode);
		}

		else if (selectMenu == 2) {
			myBorrowList(clientNode,borrowHeadNode);
		}

		else if (selectMenu == 3) {
			modifyInfo(clientNode);

		}

		else if (selectMenu == 4) {
			eraseMem(clientNode, borrowHeadNode);
		}

		else if (selectMenu == 5) {
			printf("로그아웃 되었습니다.\n");
			return;
		}

		else if (selectMenu==6) {
			printf("프로그램 종료\n");
			exit(0);
		}
	}


}

void searchBook(clientStruct* clientNode,bookStruct* bookHeadNode, Borrow* borrowHeadNode) {
	int selectMenu;

	while (1) {
		printf(">>도서 검색<<\n");
		printf("1.도서명 검색\t2.출판사 검색\n3.ISBN 검색\t4.저자명 검색\n5.전체검색\t6.이전메뉴\nselect : ");
		scanf("%d", &selectMenu);

		if (selectMenu == 1) {
			searchBookName(bookHeadNode);
		}

		else if (selectMenu == 2) {
			searchPublisher(bookHeadNode);
		}

		else if (selectMenu == 3) {
			searchISBN(bookHeadNode);
		}

		else if (selectMenu == 4) {
			searchAuthor(bookHeadNode);
		}

		else if (selectMenu == 5) {
			searchAll(bookHeadNode);
		}

		else if (selectMenu == 6) {
			return;
		}

	}
}

void searchBookName(bookStruct* bookheadNode) {
	char tempString[50];
	bookStruct* temphead = bookheadNode;
	printf("도서명을 입력하시오 : ");
	scanf("%s", tempString);

	while (temphead->nextNode != NULL) {
		if (strcmp(temphead->bookName, tempString) == 0) {
			printf("도서명 : %s\n", temphead->bookName);
			printf("출판사 : %s\n", temphead->publisher);
			printf("저자명 : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("소장처 : %s\n", temphead->location);
			printf("대여 가능 여부 : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}

}

void searchAuthor(bookStruct* bookheadNode) {
	char tempString[50];
	bookStruct* temphead = bookheadNode;
	printf("저자명을 입력하시오 : ");
	scanf("%s", tempString);

	while (temphead->nextNode != NULL) {
		if (strcmp(temphead->author, tempString) == 0) {
			printf("도서명 : %s\n", temphead->bookName);
			printf("출판사 : %s\n", temphead->publisher);
			printf("저자명 : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("소장처 : %s\n", temphead->location);
			printf("대여 가능 여부 : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}
}

void searchISBN(bookStruct* bookheadNode) {
	int tempInt;
	bookStruct* temphead = bookheadNode;
	printf("ISBN을 입력하시오 : ");
	scanf("%d", &tempInt);

	while (temphead->nextNode != NULL) {
		if (temphead->ISBN == tempInt) {
			printf("도서명 : %s\n", temphead->bookName);
			printf("출판사 : %s\n", temphead->publisher);
			printf("저자명 : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("소장처 : %s\n", temphead->location);
			printf("대여 가능 여부 : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}
}

void searchPublisher(bookStruct* bookheadNode) {
	char tempString[50];
	bookStruct* temphead = bookheadNode;
	printf("저자명을 입력하시오 : ");
	scanf("%s", tempString);

	while (temphead->nextNode != NULL) {
		if (strcmp(temphead->publisher, tempString) == 0) {
			printf("도서명 : %s\n", temphead->bookName);
			printf("출판사 : %s\n", temphead->publisher);
			printf("저자명 : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("소장처 : %s\n", temphead->location);
			printf("대여 가능 여부 : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}
}

void searchAll(bookStruct* bookheadNode) {
	bookStruct* temphead = bookheadNode;
	while (temphead->nextNode != NULL) {

		printf("도서명 : %s\n", temphead->bookName);
		printf("출판사 : %s\n", temphead->publisher);
		printf("저자명 : %s\n", temphead->author);
		printf("ISBN : %d\n", temphead->ISBN);
		printf("소장처 : %s\n", temphead->location);
		printf("대여 가능 여부 : %c\n", temphead->canBorrow);


		temphead = temphead->nextNode;

	}
}



void myBorrowList(clientStruct* clientNode, Borrow* borrowHeadNode) {

	Borrow* tempHead = borrowHeadNode;

	while (tempHead->next != NULL) {
		if (tempHead->studentNumber == clientNode->studentNumber) {
			printf("-------- 내 대여 도서 목록 --------\n");
			printf("도서 번호 : %08d\n", tempHead->bookNumber);
			printf("대여 일짜 : %d\n", tempHead->borrowTime);
			printf("반납 일짜 : %d\n", tempHead->returnTime);
			break;
		}
		else {
			tempHead = tempHead->next;
		}




	}
}

void modifyInfo(clientStruct* clientNode) {
	int selectNum;
	char tempString[100];
	


	printf("1.비밀번호\t2.주소\t3.전화번호\nselect : ");
	scanf("%d", &selectNum);

	if (selectNum == 1) {
		printf("새 비밀번호 입력 : ");
		scanf("%s", tempString);
		strcpy(clientNode->password, tempString);
		printf("비밀번호 변경완료\n");
		return;

	}

	else if (selectNum == 1) {
		printf("새 주소 입력 : ");
		scanf("%s", tempString);
		strcpy(clientNode->address, tempString);
		printf("주소 변경완료\n");
		return;

	}

	else if (selectNum == 3) {
		printf("새 전화번호 입력 : ");
		scanf("%s", tempString);
		strcpy(clientNode->address, tempString);
		printf("전화번호 변경완료\n");
		return;
	}

	else { ; }
	



}

void eraseMem(clientStruct* clientNode, Borrow* borrowHeadNode) {
	Borrow* tempHead = borrowHeadNode;
	clientStruct* tempClientHead = clientHeadNode;

	while (tempHead->next != NULL) {
		if (tempHead->studentNumber == clientNode->studentNumber) {
			printf("대여중인 도서가 있으므로 회원탈퇴가 불가합니다.");
			return;
		}
		else {
			tempHead = tempHead->next;
		}


	}
	while (tempClientHead != clientNode) {
		if(tempClientHead->nextNode==clientNode){
			tempClientHead->nextNode = clientNode->nextNode;
			free(clientNode);
			printf("회원 탈퇴가 완료되었습니다.");
			main();
		}

		else {
			tempClientHead = tempClientHead->nextNode;
		}
	}


	
	
}

