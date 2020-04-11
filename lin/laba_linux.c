#include "headers.h"

void CreateControlThread(struct Data *data)
{
	pthread_mutex_init(&(data->pMutex), NULL);
	data->count = 0;
}

void DelControlThread(struct Data *data)
{
	pthread_mutex_destroy(&(data->pMutex));
}

void* ThreadFun(void* fdata)
{
	struct Data data = *(struct Data *)fdata;
	free(fdata);
	while (1)
	{
		pthread_mutex_lock(&(data.pMutex)); 
		printf("Thread: %d of %d\n",data.count, sizeh);
		pthread_mutex_unlock(&(data.pMutex));
		usleep(100000);
	}
}

void CreateNewThread(struct Data *data, struct Stack **stack)
{
	sizeh = data->count;
	struct Data *tempData = (struct Data *)malloc(sizeof(struct Data));
	*tempData = *data;
	pthread_create(&(tempData->pThread), NULL, &ThreadFun, tempData);
	push(stack, *tempData);
}

void CloseThread(struct Stack **stack,struct Data *data, int code)
{
	if (code==0)
	{
		data->count--;
		sizeh = data->count;
		pthread_cancel((*stack)->pThread);
		pop(*&stack);
	}
	else if (code==1)
	{

		while(*stack)
		{
			pthread_cancel((*stack)->pThread);
			pop(*&stack);
			data->count--;
		}
		pthread_mutex_destroy(&(data->pMutex));
		system("clear");
	}
}

char _getch()
{
	struct termios old, new;
	char ch;
	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	ch = getchar();

	tcsetattr(0, TCSANOW, &old);
	return ch;
}

