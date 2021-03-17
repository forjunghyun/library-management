#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define time_t int

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

typedef struct {
    int studentNumber;
    int bookNumber;
    time_t borrowTime;
    time_t returnTime;

    struct Borrow* next;

}Borrow;

typedef struct __clientStruct
{
    int studentNumber;
    char* password;
    char* name;
    char* address;
    char* phoneNumber;

    struct __clientStruct* nextNode;

} clientStruct;

