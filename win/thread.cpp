#pragma once

#include "stack.h"

int lastThread;
BOOL flag = TRUE;

void CreateControlThread(struct Data *data)
{
	InitializeCriticalSection(&cs1);
	data->count = 0;
}
/*void DelControlThread(struct Data *data)
{
}*/



unsigned CALLBACK ThreadFun(void* fdata)
{
	struct Data data = *(struct Data *)fdata;
	
	while (TRUE)
	{
		Sleep(100);
		if (TryEnterCriticalSection(&cs1))
		{
			printf("Thread: %d of %d\n", data.count, sizeh);
			Sleep(500);
			LeaveCriticalSection(&cs1);
		}
		if (lastThread == data.count)
		{
			flag = TRUE;
			lastThread = 0;
			break;
		}
	}
	return 0;
}

void CreateNewThread(struct Data *data, struct Stack **stack)
{
	sizeh = data->count;
	HANDLE hThread;
	hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadFun, data, 0, NULL);
	data->hand = hThread;
	push(*&stack, *data);
}

void CloseThread(struct Stack **stack, struct Data *data, int code)
{
	if (code == 0 && flag)
	{
		flag = FALSE;
		lastThread = data->count;
		data->count--;
		sizeh = data->count;
		pop(*&stack);
	}
	else if (code == 1)
	{

		while (*stack)
		{
			TerminateThread((*stack)->hand, 0);
			pop(*&stack);
		}
		DeleteCriticalSection(&cs1);
		system("cls");
	}
}

