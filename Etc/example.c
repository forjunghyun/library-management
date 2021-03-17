#include <stdio.h>
#include <string.h>

typedef struct
{
    int studentNum;
    char name[20];
    char address[20];
    char hobby[20];
    char food[20];
    studentStruct* nextNode;
    // ppt에 있는대로 바꾸기 - clientStruct
} studentStruct;

// 전역으로 client, book, borrow 개수 변수
int clientCount, bookCount, borrowCount;

int main(void)
{
    FILE *file = fopen("client.txt", "r");

    studentStruct student[3];

    // 입력받을 때 임시로 사용할 변수 선언
    int studentNum;
    char name[20];
    char address[20];
    char hobby[20];
    char food[20];
    
    clientStruct *clientNode = (clientStruct*)malloc(sizeof(clientCount));

    // input - EOF 감지될 때까지 반복.
    for (int i = 0;; i++)
    {
        // 한줄씩 입력받음.
        fscanf(file, "%d %s %s %s %s", &studentNum, name, address, hobby, food);
         clientNode.name
       clientNode.studentNumber
       clientNode.~~~
        clientCount++;
        studentStruct* nextNode= malloc(sizeof(studentStruct));
        studentNode.nextNode = nextNode;
        studentNode = nextNode;
      
    }
    for (int i =0;i<clientCount;i++){
        출력하는 부분
    }
    
    for (int i =0;i<clientCount;i++){
        if (clientList)
    }

    return 0;
}
