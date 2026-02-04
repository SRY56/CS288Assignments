#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define NxN (N*N)
#define TRUE 1
#define FALSE 0

struct node {
	int tiles[N][N];
	int f, g, h;
	short zero_row, zero_column;
	struct node *next;
	struct node *parent;
};

int goal_rows[NxN];
int goal_columns[NxN];
struct node *start, *goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];

void print_a_node(struct node *pnode) {
	int i,j;
	for(i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
			printf("%2d ", pnode->tiles[i][j]);
		printf("\n");
	}
	printf("\n");
}

struct node *initialize(char **argv) {
	int i,j,k,index,tile;
	struct node *pnode;

	pnode = (struct node *)malloc(sizeof(struct node));
	index = 1;
	for (j = 0; j < N; j++)
		for (k = 0; k < N; k++) {
			tile = atoi(argv[index++]);
			pnode->tiles[j][k]=tile;
			if (tile == 0) {
				pnode->zero_row = j;
				pnode->zero_column = k;
			}
		}
	pnode->f = 0;
	pnode->g = 0;
	pnode->h = 0;
	pnode->next = NULL;
	pnode->parent = NULL;
	start = pnode;
	printf("Initial state\n");
	print_a_node(start);

	pnode = (struct node *)malloc(sizeof(struct node));
	goal_rows[0] = 3;
	goal_columns[0] = 3;
	
	for(index = 1; index < NxN; index++) {
		j = (index - 1) / N;
		k = (index - 1) % N;
		goal_rows[index] = j;
		goal_columns[index] = k;
		pnode->tiles[j][k] = index;
	}
	pnode->tiles[N-1][N-1] = 0;
	pnode->f = 0;
	pnode->g = 0;
	pnode->h = 0;
	pnode->next = NULL;
	goal = pnode;
	printf("goal state\n");
	print_a_node(goal);

	return start;
}

// marge remaining nodes pointed at succ_nodes in open list
// insert nodes based on f values.
void merge_to_open() {
	int i;
	struct node *curr, *prev;

	for (i = 0; i < 4; i++) {
		if (succ_nodes[i] == NULL) continue;

		// if open list is empty or if new node has smallest f value
		if (open == NULL || succ_nodes[i]->f < open->f) {
			succ_nodes[i]->next = open;
			open = succ_nodes[i];
		}
		else {
			// find correct position to insert based on f value
			prev = open;
			curr = open->next;
			while(curr != NULL && curr->f <= succ_nodes[i]->f) {
				prev = curr;
				curr = curr->next;
			}
			succ_nodes[i]->next = curr;
			prev->next = succ_nodes[i];
		}
	}
}

// swap two tiles in node
void swap (int row1, int column1, int row2, int column2, struct node * pnode) {
	int tile = pnode->tiles[row1][column1];
	pnode->tiles[row1][column1] = pnode->tiles[row2][column2];
	pnode->tiles[row2][column2] = tile;
}

// update f,g.h val for node
void update_fgh(int i) {
	struct node *pnode = succ_nodes[i];
	int j, k, tile;
	int manhattan_dist = 0;

	if (pnode == NULL) return;

	// calculate manhattan distance heuristic
	for (j = 0; j < N; j++) {
		for (k = 0; k < N; k++) {
			tile = pnode->tiles[j][k];
			if (tile != 0) {
				manhattan_dist += abs(j - goal_rows[tile]) + abs(k - goal_columns[tile]);
			}
		}
	}

	pnode->h = manhattan_dist;
	pnode->g = pnode->parent->g + 1;
	pnode->f = pnode->g + pnode->h;
}

// 0 goes down by a row
void move_down(struct node * pnode) {
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row+1, pnode->zero_column, pnode);
	pnode->zero_row++;
}

// 0 moves right by column
void move_right(struct node * pnode) {
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column+1, pnode);
	pnode->zero_column++;
}

// 0 moves up by row
void move_up(struct node * pnode) {
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row-1, pnode->zero_column, pnode);
	pnode->zero_row--;
}

// 0 goes left by column
void move_left(struct node *pnode) {
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column-1, pnode);
	pnode->zero_column--;
}


// expand node, get children nodes, organize children nodes using
// array succ_nodes
void expand(struct node *selected) {
	int i;
	struct node *pnode;

	// initialize all successor nodes to NULL
	for (i = 0; i < 4; i++) {
		succ_nodes[i] = NULL;
	}

	// move up;
	if (selected->zero_row > 0) {
		pnode = (struct node *)malloc(sizeof(struct node));
		memcpy(pnode->tiles, selected->tiles, sizeof(int)*NxN);
		pnode->zero_row = selected->zero_row;
		pnode->zero_column = selected->zero_column;
		pnode->parent = selected;
		pnode->next = NULL;
		move_up(pnode);
		succ_nodes[0] = pnode;
	}

	// move down
	if (selected->zero_row < N-1) {
		pnode = (struct node *)malloc(sizeof(struct node));
	       	memcpy(pnode->tiles, selected->tiles, sizeof(int)*NxN);
	       	pnode->zero_row = selected->zero_row;
		pnode->zero_column = selected->zero_column;
		pnode->parent = selected;
		pnode->next = NULL;
	       	move_down(pnode);
	       	succ_nodes[1] = pnode;
	}

	// move left
	if (selected->zero_column > 0) {
		pnode = (struct node *)malloc(sizeof(struct node));
	      	memcpy(pnode->tiles, selected->tiles, sizeof(int)*NxN);
		pnode->zero_row = selected->zero_row;
	       	pnode->zero_column = selected->zero_column;
	       	pnode->parent = selected;
	       	pnode->next = NULL;
	       	move_left(pnode);
	       	succ_nodes[2] = pnode;
	}

	// move right
	if (selected->zero_column < N-1) {
		pnode = (struct node *)malloc(sizeof(struct node));
		memcpy(pnode->tiles, selected->tiles, sizeof(int)*NxN);
	       	pnode->zero_row = selected->zero_row;
		pnode->zero_column = selected->zero_column;
	       	pnode->parent = selected;
		pnode->next = NULL;
		move_right(pnode);
		succ_nodes[3] = pnode;
	}
}

int nodes_same(struct node *a, struct node *b) {
	int flg=FALSE;
	if (memcmp(a->tiles, b->tiles, sizeof(int)*NxN) == 0)
		flg = TRUE;
	return flg;
}

// Filtering
void filter(int i, struct node *pnode_list) {
	struct node *pnode = succ_nodes[i];
	struct node *curr;

	if (pnode == NULL) return;

	curr = pnode_list;
	while(curr != NULL) {
		if (nodes_same(pnode, curr)) {
			// found dups, free successor node
			free(succ_nodes[i]);
			succ_nodes[i] = NULL;
			return;
		}
		curr = curr->next;
	}
}

int main (int argc, char **argv) {
	int iter, cnt;
	struct node *copen, *cp, *solution_path;
	int ret, i, pathlen=0, index[N-1];

	if (argc != 17) {
		printf("Useage: %s tile0 tile1 ... tile15\n", argv[0]);
		printf("where tiles are numbers 0-15 in row major order\n");
		return 1;
	}

	solution_path = NULL;
	start = initialize(argv);
	open = start;

	iter = 0;
	while (open != NULL) {
		copen = open;
		open = open->next;

		if (nodes_same(copen, goal)) {
			do {
				copen->next = solution_path;
				solution_path = copen;
				copen = copen->parent;
				pathlen++;
			} while(copen != NULL);
			printf("Path (length=%d):\n", pathlen);
			copen = solution_path;
			while(copen != NULL) {
				print_a_node(copen);
				copen = copen->next;
			}
			break;
		}
		expand(copen);

		for (i = 0; i < 4; i++) {
			filter(i, open);
			filter(i, closed);
			update_fgh(i);
		}

		merge_to_open();

		copen->next = closed;
		closed=copen;
		
		iter++;
		//if (iter %1000 == 0)
			//printf("%2d", iter);
	}
	//printf("\n");

	if(open == NULL && solution_path == NULL) {
		printf("no solution\n");
	}

	return 0;
}
