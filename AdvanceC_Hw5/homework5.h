#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <crtdbg.h>

#define FLUSH while (getchar () != '\n')
#define MEM_ERR printf("Not enough memory\n"), getchar(), exit(100)
#define FILE_ERROR printf("error opening file\n"), getchar(), exit(101)
#define ERR_NO_MEMORY     printf(" ERROR, not enough memory\n"), getchar(), exit (100)
#define ERR_OPENING_FILE  printf(" Error Opening File!!!\n"), getchar(), exit(102)
#define ERR_CLOSING_FILE  printf(" Error Closing File!!!\n"), getchar(), exit(103)

typedef struct queueList{
	int line;
	queueList *next;
}QUEUELIST;

typedef struct{
	QUEUELIST *queue;
	QUEUELIST *rear;
} QUEUE;

typedef struct node{
	node *left;
	char identifier[25];
	QUEUE queue;
	node *right;
} NODE;

typedef struct{
	NODE *root;
	//int   count;
} HEADER;

int insert( NODE **root, char *data, int line, int (*comp)(void*, void*));
void print(FILE* output, NODE* node);
NODE *find(NODE *root, char* target, int (*comp)(void*, void*));
void enqueue(QUEUELIST **queue, QUEUELIST **rear, int data);
NODE *destroy(NODE *root);