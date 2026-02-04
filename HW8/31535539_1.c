#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node *next;
};

int main() {
	struct node *head = NULL;
	struct node *reversed_head = NULL;
	int data;
	int count = 1;

	printf("Enter integers (ctrl-d to end):\n");

	// read integers and create original list
	while (scanf("%d", &data) != EOF) {
		// allocate memory to new node
		struct node *new_node = (struct node *)malloc(sizeof(struct node));
		if (new_node == NULL) {
			printf("Memory Allocation Failed");
			return 1;
		}

		new_node->value = data;
		new_node->next = NULL;

		// add node to end of the original list
		if (head == NULL) {
			head = new_node;
		} else {
			struct node *temp = head;
			while (temp->next != NULL) {
				temp = temp->next;
			}
			temp->next = new_node;
		}
		count++;
	}

	// print original list
	printf("\nData entered in the list are:\n");
	struct node *current = head;
	while (current != NULL) {
		printf("Data = %d\n", current->value);
		current = current->next;
	}

	// reverse list
	while (head != NULL) {
		// remove node from front of list
		struct node *tail = head;
		head = head->next;

		// add node to front of reversed list
		tail->next = reversed_head;
		reversed_head = tail;
	}

	// display reverse list
	printf("\nThe list in reverse are:\n");
	current = reversed_head;
	while (current != NULL) {
		printf("Data = %d\n", current->value);
		current = current->next;
	}

	// free space
	while (reversed_head != NULL) {
		struct node *temp = reversed_head;
		reversed_head = reversed_head->next;
		free(temp);
	}



	return 0;
}
