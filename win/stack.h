#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

#include <windows.h>
#include <process.h> 
#include <conio.h> 

static CRITICAL_SECTION cs1;
static int sizeh;
unsigned CALLBACK ThreadFun(void *);

struct Data
{
	HANDLE hand;
	CRITICAL_SECTION cs1;

	int count;
};



struct Stack {

	HANDLE hand;

	struct Stack *next;
};

void push(struct Stack **, struct Data);
void pop(struct Stack **);
