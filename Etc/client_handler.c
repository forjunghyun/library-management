#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
typedef struct __clientStruct
{
	int studentNumber;
	char* password;
	char* name;
	char* address;
	char* phoneNumber;

	struct __clientStruct* nextNode;

} clientStruct;

void saveStringByDynamicAlloc(char**, char*);
clientStruct* client_signup(clientStruct* Head);
void Client_Print(clientStruct* Head);
clientStruct* Client_StdIn(clientStruct* StpData);
void Client_StdOut(clientStruct* StpData);
clientStruct* login(clientStruct* Head);
int main(void)
{

	char tempString[999];
	char* tempTokPtr;

	clientStruct* clientHeadNode = (clientStruct*)(malloc(sizeof(clientStruct)));

	clientStruct* clientNode = (clientStruct*)(malloc(sizeof(clientStruct)));
	clientStruct* tempClientNode;


	clientHeadNode = clientNode;


	FILE* clientFileReadStream = fopen("client.txt", "r");

	while ((fscanf(clientFileReadStream, "%[^\n]", tempString)) != EOF) {
		clientNode->studentNumber = atoi(strtok(tempString, "|"));
		saveStringByDynamicAlloc(&(clientNode->password), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->name), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->address), strtok(NULL, "|"));
		saveStringByDynamicAlloc(&(clientNode->phoneNumber), strtok(NULL, "|"));

		tempClientNode = (clientStruct*)(malloc(sizeof(clientStruct)));
		clientNode->nextNode = tempClientNode;
		clientNode = tempClientNode;

		clientNode->nextNode = NULL;

		fgetc(clientFileReadStream);
	}

	clientNode = clientHeadNode;


	fclose(clientFileReadStream);

	while (clientNode->nextNode != NULL)
	{
		printf("%d | %s | %s | %s | %s\n", clientNode->studentNumber, clientNode->password, clientNode->name, clientNode->address, clientNode->phoneNumber);
		clientNode = clientNode->nextNode;
	}
	// login(clientHeadNode); //로그인 함수
	clientHeadNode = client_signup(clientHeadNode); // 회원가입 함수
	Client_Print(clientHeadNode); // 회원가입 후 출력 함수
	return 0;
}
clientStruct* login(clientStruct* Head)  // 로그인함수 - 관리자용, 학생용
{
	int res, studentNum;
	char password[50];
	char admin[50];
	printf("학번 : ");
	res = scanf("%d", &studentNum);
	if (res != 1) { // 관리자용
		scanf("%s", admin);
		if (strcmp("admin", admin) == 0)
		{
			printf("비밀번호 : ");
			scanf("%s", password);
			if (strcmp("admin123", password) == 0)
			{
				printf("관리자용 접속 성공\n");
				// 관리자용 메뉴 출력해야함
			}
		}
	}
	else {
		while (0 != Head)
		{
			if (Head->studentNumber == studentNum)
			{
				printf("비밀번호 : ");
				scanf("%s", password);
				if (strcmp(password, Head->password) == 0)
				{
					printf("회원용 로그인 성공\n");
				}
				else
				{
					printf("비밀번호가 틀렸습니다.\n");
				}
			}

			Head = Head->nextNode;
		} //학번이 아예 일치하지 않을 때 학번이 일치하지 않음 출력 
	}
}

clientStruct* client_signup(clientStruct* Head) //가입 후 정렬 함수

{
	clientStruct* StpNew = (clientStruct*)malloc(sizeof(clientStruct));
	clientStruct* StpFront = Head;
	clientStruct* StpRear = Head;

	StpNew = Client_StdIn(StpNew);
	StpNew->nextNode = 0;

	while (0 != StpRear)	//	 삽입 위치  검색 	 //중간 삽입 코드
	{
		if (StpRear->studentNumber > StpNew->studentNumber)	// 삽입 위치 판단 
		{
			break;	// insert location ! 
		}
		StpFront = StpRear;				 //Front = Rear ㅇㅇ
		StpRear = StpRear->nextNode;			 //Rear 가 한칸 이동!
	}
	if (Head != StpRear)			 //중간 혹은 끝부분 삽입 시
	{
		StpNew->nextNode = StpRear;
		StpFront->nextNode = StpNew;
	}
	else			 //앞 삽입

	{
		StpNew->nextNode = Head;
		Head = StpNew;
	}
	Client_Print(Head);
	return Head;

}

clientStruct* Client_StdIn(clientStruct* StpData) //가입정보 입력 함수
{
	StpData->password = malloc(sizeof(char) * 100);
	StpData->name = malloc(sizeof(char) * 100);
	StpData->address = malloc(sizeof(char) * 100);
	StpData->phoneNumber = malloc(sizeof(char) * 100);

	printf(" < 회원가입 > \n\n");
	printf(" 학번 : ");
	scanf("%d", &StpData->studentNumber);
	printf("비밀번호 : ");
	scanf("%s", StpData->password);
	printf("이름 : ");
	scanf("%s", StpData->name);
	printf("주소 : ");
	scanf("%s", StpData->address);
	printf("전화번호 : ");
	scanf("%s", StpData->phoneNumber);
	printf("\n");
	printf("회원가입이 완료되었습니다.\n");

	StpData->nextNode = 0;


	return StpData;
}
void Client_Print(clientStruct* Head)
{
	while (0 != Head)
	{
		Client_StdOut(Head);
		Head = Head->nextNode;
	}
}
void Client_StdOut(clientStruct* StpData)
{
	printf("%d | %s | %s | %s | %s\n", StpData->studentNumber, StpData->password, StpData->name, StpData->address, StpData->phoneNumber);
}

void saveStringByDynamicAlloc(char** destPtr, char* sourcePtr)
{
	*destPtr = (char*)malloc(strlen(sourcePtr) + 1);
	strcpy(*destPtr, sourcePtr);
}
