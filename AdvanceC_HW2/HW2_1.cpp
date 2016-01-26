/*This program fills up a double linklist consisting of players and their scores 
	and prints their name and scores on the screen and into a file
Name: Poureya Sohbati
Date: 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crtdbg.h>

typedef struct node{
	int point;
	node* next;	
}SNODE;

typedef struct dnode{

	char name[30];
	SNODE *score;
	dnode *back;
	dnode *forward;

}DNODE;

typedef struct{
	int count;
	DNODE *linklist;
}HEADER;

#define FLUSH while (getchar () != '\n')
#define ERR_NO_MEMORY     printf(" ERROR, not enough memory\n"), getchar(), exit (100)
#define ERR_OPENING_FILE  printf(" Error Opening File!!!\n"), getchar(), exit(102)
#define ERR_CLOSING_FILE  printf(" Error Closing File!!!\n"), getchar(), exit(103)

HEADER* createHeader (void);
FILE* openFile(char filename[], char mode[]);
void insertDLL(HEADER* header, DNODE* player, char* name, int point);
int searchPlayer(DNODE** ptr, char name[]);
SNODE* push(SNODE* stack, int data);
SNODE* pop(SNODE** stack);
void printAToZ(HEADER* header);
void printZToA(HEADER* header);
//void insert(HEADER* header, int score, char name[]);
//DNODE* search(HEADER* header, char name[]);
void printToFile(HEADER* header, FILE* output);
HEADER* freeMemory(HEADER* header);

int main (void)
{
	HEADER* header;
	DNODE* player;
	char filename[30];
	char outputname[15];
	FILE* input;
	FILE* output;
	char name[30];
	int point;

	printf("Enter the name of the input: ");
	scanf(" %s", filename);
	FLUSH;
	input = openFile(filename, "r");
	header = createHeader();
	while(fscanf(input, " %[^;]%*c %d", name, &point) != EOF)
	{
		player = header->linklist;
		if(!searchPlayer(&player, name))
			insertDLL(header, player, name, point);
		else
			player->score = push(player->score, point);
		
	}
	if(!fclose(input) == EOF)
		ERR_CLOSING_FILE; 
	printAToZ(header);
	printZToA(header);
	 
	while(player = header->linklist, printf("\nPlease enter the full name of the player or "
											"enter quit to exit the program.\nName Lastname(It is case sensitive): ")
											, gets(name), strcmp(name, "quit"))
	{
		if(searchPlayer(&player, name))
			printf("%s %d\n", player->name, player->score->point);
		else
			printf("Not found\n");
	}
		
	filename[strlen(filename)-4] = '\0';
	strcat(filename, "_out.txt");
	printf("\nThe output file has been saved into %s\n\n", filename);
	output = openFile(filename, "w");
	printToFile(header, output);
	if(fclose(output) == EOF)
		ERR_CLOSING_FILE;

	header = freeMemory(header);
	printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "\tNo Memory Leak\n");
	
	return 0;
}

HEADER* createHeader (void)
{
	HEADER* header;

	if(!(header = (HEADER*) malloc (sizeof(HEADER))))
		ERR_NO_MEMORY;
	if(!(header->linklist = (DNODE*) malloc (sizeof(DNODE))))
		ERR_NO_MEMORY;
	if(!(header->linklist->forward = (DNODE*) malloc (sizeof(DNODE))))
		ERR_NO_MEMORY;
	header->linklist->forward->back = header->linklist;
	header->linklist->forward->forward = header->linklist;
	header->linklist->back = header->linklist->forward;
	header->count = 0;
	strcpy(header->linklist->name, "@");
	strcpy(header->linklist->forward->name, "}");

	return header;
}

//===================== openInputFile ======================
/*
This function opens a file 
PRE: filename, opening mode
POST: opens the file
Return: pointer to the file
*/
FILE* openFile(char filename[], char mode[])
{
	FILE *input;

	if(!(input = fopen(filename, mode)))
		ERR_OPENING_FILE;

	return input;
}

//===================== insertDLL ======================
/*
This function insert a new player into the double linklist
PRE: HEADER*, DNODE*, char*, int point 
POST: enters the new player into the linklist
Return: nothing 
*/
void insertDLL(HEADER* header, DNODE* ptr, char* name, int point)
{
	DNODE* node;

	if(!(node = (DNODE*) malloc (sizeof(DNODE))))
		ERR_NO_MEMORY;
	strcpy(node->name, name);
	node->score = push(node->score, point);
	node->score->next = NULL;
	node->back = ptr->back;
	node->forward = ptr;
	ptr->back = node;
	node->back->forward = node;
	(header->count)++;

	return;
}

//DNODE* search(HEADER* header, char name[])
//{
//	DNODE* walker = header->linklist->forward;
//	int found = 0;
//
//	while((found = strcmp(walker->name, name)) < 0)
//		walker = walker->forward;
//
//	 return !found ? walker : NULL;
//}

//void insert(HEADER* header, int score, char name[])
//{
//	DNODE* ptr;
//
//	if(ptr = search(header, name))
//		ptr->score = push(ptr->score, score);
//	else
//	{
//		ptr = header->linklist;
//		while(strcmp(ptr->name, name) < 0)
//			ptr = ptr->forward;
//		DNODE* node;
//
//		if(!(node = (DNODE*) malloc (sizeof(DNODE))))
//		ERR_NO_MEMORY;
//		strcpy(node->name, name);
//		node->score = push(node->score, score);
//		node->score->next = NULL;
//		node->back = ptr->back;
//		node->forward = ptr;
//		ptr->back = node;
//		node->back->forward = node;
//		(header->count)++;
//	}
//	return;
//}

//===================== searchPlayer ======================
/*
This function searches the linklist for a name
PRE: two level pointer to the begining of the linklist
Return: true if player was found and false if the player was not found
		and returns a pointer to the player that was found or returns a
		pointer to the next player that in the link list if the player
		was not found
*/
int searchPlayer(DNODE** ptr, char name[])
{
	int found = 0;

	while((found = strcmp((*ptr)->name, name)) < 0)
		(*ptr) = (*ptr)->forward;

	return !found ?  1 : 0;
}

//===================== push ======================
/*
This function pushes a score into the stack
PRE: pointer to stack and int data
POST: pushes the data into the stack 
Return: pionter to stack
*/
SNODE* push(SNODE* stack, int data)
{
	SNODE* pNew;
	if(!(pNew = (SNODE*) malloc (sizeof(SNODE))))
		ERR_NO_MEMORY;
	pNew->point = data;
	pNew->next = stack;
	stack = pNew;

	return stack;
}

//===================== pop ======================
/*
This function pops a node from the stack
PRE: two level pointer to the stack
POST: pops the data
Return: returns a pointer to the poped node 
*/
SNODE* pop(SNODE** stack)
{
	SNODE* pFirst;
	if(!stack) 
		return NULL;
	pFirst = *stack;
	*stack = (*stack)->next;
	pFirst->next = NULL;

	return pFirst;
}

//===================== printAtoZ ======================
/*
This function prints the double linklist from A to Z 
PRE: printer to the header
POST: prints the double linklist
Return: nothing
*/
void printAToZ(HEADER* header)
{
	DNODE* pWalker;
	int i = 0;
	
	printf("Players and their most recent score from A to Z:\n");
	for(pWalker = header->linklist->forward; i < (header->count); pWalker = pWalker->forward, i++)
		printf("%s %d\n", pWalker->name, pWalker->score->point);

	return;
}

//===================== printZtoA ======================
/*
This function prints the double linklist from Z to A 
PRE: printer to the header
POST: prints the double linklist
Return: nothing
*/
void printZToA(HEADER* header)
{
	DNODE* pWalker;
	int i = header->count;

	printf("\nPlayers and their most recent score from A to Z:\n");
	for(pWalker = header->linklist->back->back; i > 0; pWalker = pWalker->back, i--)
		printf("%s %d\n", pWalker->name, pWalker->score->point);

	return;
}

//===================== printToFile ======================
/*
This function prints the players name and all of their scores and
	 their average score into a file
PRE: pointer to the header, FIlE*
POST: prints the data
Return: nothing
*/
void printToFile(HEADER* header, FILE* output)
{
	DNODE* pWalker;
	SNODE* snode;
	int i = 0;
	double sum;
	int num;

	for(pWalker = header->linklist->forward; i < header->count; pWalker = pWalker->forward, i++){
		fprintf(output, "%-30s ", pWalker->name);
		sum = 0;
		num = 0;
		while(pWalker->score){
			snode = pop(&pWalker->score);
			sum += snode->point;
			num++;
			fprintf(output, "%2d ", snode->point);
			free(snode);
		}
		fprintf(output, "%.2f\n", sum/num);
	}
	return;
}

//===================== freeMemory ======================
/*
This function freees the memory
PRE: pointer to the header
POST: free the memory
Return: NULL 
*/
HEADER* freeMemory(HEADER* header)
{
	DNODE* walker;
	int i = 0;
	DNODE* curr;

	walker = header->linklist->forward;
	curr = header->linklist;
	while(free(curr), i++ < header->count + 1)
	{
		 curr = walker;
		 walker = walker->forward;
	}
	free(header);

	 
	return NULL;
}
/*
Enter the name of the input: players.txt
Players and their most recent score from A to Z:
Anthony Davis 34
Blake Griffin 30
Chris Paul 25
Damian Lillard 16
James Harden 16
Kevin Durant 38
Kevin Love 43
LaMarcus Aldridge 18
LeBron James 17

Players and their most recent score from A to Z:
LeBron James 17
LaMarcus Aldridge 18
Kevin Love 43
Kevin Durant 38
James Harden 16
Damian Lillard 16
Chris Paul 25
Blake Griffin 30
Anthony Davis 34

Please enter the full name of the player or enter quit to exit the program.
Name Lastname(It is case sensitive): LeBron James
LeBron James 17

Please enter the full name of the player or enter quit to exit the program.
Name Lastname(It is case sensitive): Damian Lillard
Damian Lillard 16

Please enter the full name of the player or enter quit to exit the program.
Name Lastname(It is case sensitive): sadfasf
Not found

Please enter the full name of the player or enter quit to exit the program.
Name Lastname(It is case sensitive): Blake Griffin
Blake Griffin 30

Please enter the full name of the player or enter quit to exit the program.
Name Lastname(It is case sensitive): quit

The output file has been saved into players_out.txt

        No Memory Leak
*/