#pragma once
#include "stack.h"


void push(struct Stack **head, struct Data data) {

	struct Stack *tmp = (struct Stack *)malloc(sizeof(struct Stack));
	if (tmp == NULL)
		exit(1);
	tmp->next = *head;


	tmp->hand = data.hand;

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
