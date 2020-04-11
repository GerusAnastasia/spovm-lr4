#pragma once

#include "stack.h"


void CreateNewThread(struct Data *, struct Stack **);
void CloseThread(struct Stack **, struct Data *, int code);
void CreateControlThread(struct Data *);

int main(int argc, char *argv[])
{
	struct Stack *stack = NULL;
	struct Data data;

	CreateControlThread(&data);
	while (1)
	{
		switch (_getch())
		{
		case '+':
			data.count++;
			CreateNewThread(&data, &stack);
			break;
		case '-':
			if (stack) {
				CloseThread(&stack, &data, 0);
			}
			break;
		case 'q':
			CloseThread(&stack, &data, 1);
			return 0;
			break;
		}
	}

	//DelControlThread(&data);

	free(stack);
	return 0;
}
