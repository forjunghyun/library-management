#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#define time_t int
#pragma warning(disable : 4996)

typedef struct {
	int studentNumber;
	int bookNumber;
	time_t borrowTime;
	time_t returnTime;

	struct Borrow* next;

}Borrow;

void searchBookInfo(Borrow*);
void showBorrowList(Borrow*);

int main() {
	char tempString[100];
	Borrow* BorrowHeadNode = (Borrow*)malloc(sizeof(Borrow));
	Borrow* BorrowNode = (Borrow*)malloc(sizeof(Borrow));
	Borrow* tempBorrowNode;
	BorrowHeadNode = BorrowNode;
	char* token;

	FILE* borrowFileLoad = fopen("borrow.txt", "r");

	if (borrowFileLoad == NULL) {
		printf("파일 읽기 실패.\n");
	}
	else { ; }

	while (1) {
		fscanf(borrowFileLoad, "%s",tempString);
		if (feof(borrowFileLoad)) { break; }
		/*printf("%s\n", tempString);*/

		token = strtok(tempString, "|");
		BorrowNode->studentNumber = atoi(token);
		token = strtok(NULL, "| ");
		BorrowNode->bookNumber = atoi(token);
		token = strtok(NULL, "| ");
		BorrowNode->borrowTime = atoi(token);
		token = strtok(NULL, "");
		BorrowNode->returnTime = atoi(token);

		
		
		tempBorrowNode = (Borrow*)malloc(sizeof(Borrow));
		BorrowNode->next = tempBorrowNode;
		BorrowNode = tempBorrowNode;

		BorrowNode->next = NULL;

		


	}

	fclose(borrowFileLoad);

	showBorrowList(BorrowHeadNode);
	searchBookInfo(BorrowHeadNode);

	



	


	
}

void showBorrowList(Borrow* BorrowHeadNode) {
	printf("-------- 내 대여목록 --------\n");
	while (BorrowHeadNode->next != NULL) {
		printf("%d | %08d | %d | %d\n", BorrowHeadNode->studentNumber, BorrowHeadNode->bookNumber, BorrowHeadNode->borrowTime, BorrowHeadNode->returnTime);
		BorrowHeadNode = BorrowHeadNode->next;
	}
	

}


void searchBookInfo(Borrow* searchBook) {
	int searchNum;
	int i;
	printf("학번을 입력하십시오 : ");
	scanf("%d", &searchNum);

	while (searchBook->next != NULL) {
		if (searchBook->studentNumber == searchNum) {
			printf("-------- 대여 도서 목록 --------\n");
			printf("도서 번호 : %08d\n", searchBook->bookNumber);
			printf("대여 일짜 : %d\n", searchBook->borrowTime);
			printf("반납 일짜 : %d\n", searchBook->returnTime);
			break;
		}
		else {
			searchBook = searchBook->next;
		}
	}


	return;


}