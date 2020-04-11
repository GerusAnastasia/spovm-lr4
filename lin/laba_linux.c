#define _CRT_SECURE_NO_WARNINGS

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

char _getch();

int sizeh;
struct Data
{

	pthread_mutex_t pMutex;
	pthread_t pThread;

	int count;
};


void CreateNewThread(struct Data *, struct Stack **);
void CloseThread(struct Stack **, struct Data *, int code);
void DelControlThread(struct Data *);
void CreateControlThread(struct Data *);

struct Stack {

	pthread_t pThread;

	struct Stack *next;
};

void push(struct Stack **head, struct Data data) {

	struct Stack *tmp = (struct Stack *)malloc(sizeof(struct Stack));
	if (tmp == NULL)
		exit(1);
	tmp->next = *head;
	
   
	tmp->pThread = data.pThread;

	*head = tmp;
}

void pop(struct Stack **head) {

	struct Stack *out;

	if (*head == NULL)
		exit(1);

	out = *head;
	*head = (*head)->next;
	
	free(out);
}



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
		usleep(500000);
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
	struct termios old, neww;
	char ch;
	tcgetattr(0, &old);
	neww = old;
	neww.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &neww);
	ch = getchar();

	tcsetattr(0, TCSANOW, &old);
	return ch;
}




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
				if (stack){
					CloseThread(&stack, &data,0);
					}
				break;	
			case 'q':
					CloseThread(&stack, &data,1);
				return 0;
				break;
			}
		}

		DelControlThread(&data);
		//free(stack);
	return 0;
}
