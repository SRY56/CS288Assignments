#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node *next;
};

// function to create new node
struct node *create_node(int value) {
	struct node *new_node = (struct node *)malloc(sizeof(struct node));
	new_node->value = value;
	new_node->next = NULL;
	return new_node;
}

// function to insert a node at end of list
void insert_node(struct node **head, int value) {
	struct node *new_node = create_node(value);

	if (*head == NULL) {
		*head = new_node;
		return;
	}

	struct node *temp = *head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = new_node;
}

// bubble sort
// swaps nodes by relinking them
void bubble_sort(struct node **head) {
	if (*head == NULL || (*head)->next == NULL) return;

	int swapped;

	do {
		swapped = 0;
		struct node *prev = NULL;
		struct node *current = *head;
		struct node *next = current->next;

		while (next != NULL) {
			// if current val is greater then next val, swap nodes
			if (current->value > next->value) {
				// perform swap
				current->next = next->next;
				next->next = current;

				if (prev != NULL) {
					prev->next = next;
				} else {
					*head = next;
				}

				swapped = 1;

				// update pointers after swap
				// order if now: prev -> next -> current -> current->next
				prev = next;
				next = current->next;
			} else {
				// no swap, move pointers forward
				prev = current;
				current = next;
				next = next->next;
			}
		}
	} while (swapped);
}

// function to print linked list
void print_list(struct node *head) {
	struct node *temp = head;
	while (temp != NULL) {
		printf("%d\n", temp->value);
		temp = temp->next;
	}
}

// void free linked list
void free_list(struct node *head) {
	struct node *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

int main() {
	struct node *head = NULL;
	int value;
	int count = 0;

	// read integers until (ctrl-d)
	printf("Enter integers (Ctrl-D to finish):\n");
	while (scanf("%d", &value) != EOF) {
		insert_node(&head, value);
		count++;
	}

	// sort linked list using bubble sort
	bubble_sort(&head);

	printf("\nSorted list: \n");
	// print sorted list
	print_list(head);

	// free allocated memory
	free_list(head);

	return 0;
}
