#pragma once
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#pragma warning(disable  : 4996)
#include"LibraryMagagement.h"



//�����б� ���α׷�--------------------------------------------------------------------------------------------
clientStruct* clientHeadNode;
clientStruct* clienttail;
bookStruct* bookHeadNode;
bookStruct* booktail;
Borrow* BorrowHeadNode;
Borrow* Borrowtail;



void file_read()
{
	FILE* client_file, * book_file, * borrow_file;
	//client ����
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

	//borrow ����
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






//ȸ���� ���α׷�--------------------------------------------------------------------------------------------
void memberRegister(clientStruct* clientHeadNode); //ȸ�����
void loginMember(clientStruct* clientHeadNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode); //�α��θ޴�
void loginScreen(clientStruct* clientNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode);//�α����� �� ȸ���޴�â
void searchBook(clientStruct* clientNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode); //�����˻�
void myBorrowList(clientStruct* clientNode, Borrow* borrowHeadNode); //�� �뿩���
void modifyInfo(clientStruct* clientNode);//������������
void eraseMem(clientStruct* clientNode, Borrow* borrowHeadNode);//ȸ��Ż��
//�����˻� �޴�----------------------------------------------------------------------------------------------
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
		printf(">>������ ����<<\n");
		printf("1.ȸ������\t2.�α���\t2.���α׷�����\select : ");
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

	printf(">>ȸ������<<\n");

	while (true) {
		printf("�й� �Է� : ");
		scanf("%d", &tempInt);

		temphead = clientHeadNode;
		while (temphead->nextNode != NULL) {

			if (temphead->studentNumber == tempInt) {
				printf("�̹� ��ϵǾ��ִ� ȸ���Դϴ�.\n");
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

	printf("��� ��ȣ �Է� : ");
	scanf("&s", tempString);
	strcpy(newMember->password, tempString);

	printf("�̸� �Է� : ");
	scanf("%s", tempString);
	strcpy(newMember->name, tempString);

	printf("�ּ� �Է� : ");
	strcpy(newMember->address, tempString);

	printf("��ȭ ��ȣ �Է� : ");
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
	
	printf(">>�α��� ȭ��<<\n");
	while (true){
		temphead = clientHeadNode;
		printf("�α��θ�(�й�) : ");
		scanf("%d", &tempInt);
		printf("��ι�ȣ �Է� : ");
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
			printf("���� ȸ���Դϴ�.");
		}
	}

}

void loginScreen(clientStruct* clientNode, bookStruct* bookHeadNode, Borrow* borrowHeadNode) {
	int selectMenu;

	while (1) {
		printf(">>ȸ�� �޴�<<\n");
		printf("1.�����˻�\t2.�� �뿩���\n3.�������� ����\t4.ȸ��Ż��\n5.�α׾ƿ�\t6.���α׷�����\nselect : ");
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
			printf("�α׾ƿ� �Ǿ����ϴ�.\n");
			return;
		}

		else if (selectMenu==6) {
			printf("���α׷� ����\n");
			exit(0);
		}
	}


}

void searchBook(clientStruct* clientNode,bookStruct* bookHeadNode, Borrow* borrowHeadNode) {
	int selectMenu;

	while (1) {
		printf(">>���� �˻�<<\n");
		printf("1.������ �˻�\t2.���ǻ� �˻�\n3.ISBN �˻�\t4.���ڸ� �˻�\n5.��ü�˻�\t6.�����޴�\nselect : ");
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
	printf("�������� �Է��Ͻÿ� : ");
	scanf("%s", tempString);

	while (temphead->nextNode != NULL) {
		if (strcmp(temphead->bookName, tempString) == 0) {
			printf("������ : %s\n", temphead->bookName);
			printf("���ǻ� : %s\n", temphead->publisher);
			printf("���ڸ� : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("����ó : %s\n", temphead->location);
			printf("�뿩 ���� ���� : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}

}

void searchAuthor(bookStruct* bookheadNode) {
	char tempString[50];
	bookStruct* temphead = bookheadNode;
	printf("���ڸ��� �Է��Ͻÿ� : ");
	scanf("%s", tempString);

	while (temphead->nextNode != NULL) {
		if (strcmp(temphead->author, tempString) == 0) {
			printf("������ : %s\n", temphead->bookName);
			printf("���ǻ� : %s\n", temphead->publisher);
			printf("���ڸ� : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("����ó : %s\n", temphead->location);
			printf("�뿩 ���� ���� : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}
}

void searchISBN(bookStruct* bookheadNode) {
	int tempInt;
	bookStruct* temphead = bookheadNode;
	printf("ISBN�� �Է��Ͻÿ� : ");
	scanf("%d", &tempInt);

	while (temphead->nextNode != NULL) {
		if (temphead->ISBN == tempInt) {
			printf("������ : %s\n", temphead->bookName);
			printf("���ǻ� : %s\n", temphead->publisher);
			printf("���ڸ� : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("����ó : %s\n", temphead->location);
			printf("�뿩 ���� ���� : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}
}

void searchPublisher(bookStruct* bookheadNode) {
	char tempString[50];
	bookStruct* temphead = bookheadNode;
	printf("���ڸ��� �Է��Ͻÿ� : ");
	scanf("%s", tempString);

	while (temphead->nextNode != NULL) {
		if (strcmp(temphead->publisher, tempString) == 0) {
			printf("������ : %s\n", temphead->bookName);
			printf("���ǻ� : %s\n", temphead->publisher);
			printf("���ڸ� : %s\n", temphead->author);
			printf("ISBN : %d\n", temphead->ISBN);
			printf("����ó : %s\n", temphead->location);
			printf("�뿩 ���� ���� : %c\n", temphead->canBorrow);
		}
		else {
			temphead = temphead->nextNode;
		}
	}
}

void searchAll(bookStruct* bookheadNode) {
	bookStruct* temphead = bookheadNode;
	while (temphead->nextNode != NULL) {

		printf("������ : %s\n", temphead->bookName);
		printf("���ǻ� : %s\n", temphead->publisher);
		printf("���ڸ� : %s\n", temphead->author);
		printf("ISBN : %d\n", temphead->ISBN);
		printf("����ó : %s\n", temphead->location);
		printf("�뿩 ���� ���� : %c\n", temphead->canBorrow);


		temphead = temphead->nextNode;

	}
}



void myBorrowList(clientStruct* clientNode, Borrow* borrowHeadNode) {

	Borrow* tempHead = borrowHeadNode;

	while (tempHead->next != NULL) {
		if (tempHead->studentNumber == clientNode->studentNumber) {
			printf("-------- �� �뿩 ���� ��� --------\n");
			printf("���� ��ȣ : %08d\n", tempHead->bookNumber);
			printf("�뿩 ��¥ : %d\n", tempHead->borrowTime);
			printf("�ݳ� ��¥ : %d\n", tempHead->returnTime);
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
	


	printf("1.��й�ȣ\t2.�ּ�\t3.��ȭ��ȣ\nselect : ");
	scanf("%d", &selectNum);

	if (selectNum == 1) {
		printf("�� ��й�ȣ �Է� : ");
		scanf("%s", tempString);
		strcpy(clientNode->password, tempString);
		printf("��й�ȣ ����Ϸ�\n");
		return;

	}

	else if (selectNum == 1) {
		printf("�� �ּ� �Է� : ");
		scanf("%s", tempString);
		strcpy(clientNode->address, tempString);
		printf("�ּ� ����Ϸ�\n");
		return;

	}

	else if (selectNum == 3) {
		printf("�� ��ȭ��ȣ �Է� : ");
		scanf("%s", tempString);
		strcpy(clientNode->address, tempString);
		printf("��ȭ��ȣ ����Ϸ�\n");
		return;
	}

	else { ; }
	



}

void eraseMem(clientStruct* clientNode, Borrow* borrowHeadNode) {
	Borrow* tempHead = borrowHeadNode;
	clientStruct* tempClientHead = clientHeadNode;

	while (tempHead->next != NULL) {
		if (tempHead->studentNumber == clientNode->studentNumber) {
			printf("�뿩���� ������ �����Ƿ� ȸ��Ż�� �Ұ��մϴ�.");
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
			printf("ȸ�� Ż�� �Ϸ�Ǿ����ϴ�.");
			main();
		}

		else {
			tempClientHead = tempClientHead->nextNode;
		}
	}


	
	
}

