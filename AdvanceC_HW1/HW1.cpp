/*This program expects the name of an input file (art.txt) and two output files
to be given by the user. The input file has lines which look like this:
Dan Brown
^ ^
(O,O)
( )
-"-"---
# 3
and the program sorts the artists by their names and finds the highest score.
	Name: Pourya Sohbati
	Date: 2/17/14
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crtdbg.h>

#define MAX_STRING 100
#define MAX_PICTURE 1600
#define MAX_FILE 20
#define MAX_ARR 1000
#define FLUSH while (getchar () != '\n')
#define ERR_NO_MEMORY     printf(" ERROR, not enough memory\n"), getchar(), exit (100)
#define ERR_OPENING_FILE  printf(" Error Opening File!!!\n"), getchar(), exit(102)
#define ERR_CLOSING_FILE  printf(" Error Closing File!!!\n"), getchar(), exit(103)

typedef struct{
	char *artist;
	char *cartoon;
	int   score;
}ART;

FILE* openFile(char filename[], char mode[]);
char* allocateString (char stName[]);
ART* createArray (int size);
void replaceN (char str[]);
void saveToArt (char name[], char picture[], int score, ART* artPtr);
void print (ART* art, int size);
void selectionSortAscending(ART* myArr, int size);
void saveToFile(FILE* output, ART* art, int size);
char* prompt(char arr[]);
void highestScore(FILE* output, ART* art, int size);
ART* freeMemory(ART* art, int size);

int main (void)
{
	//Local Declaration
	char filename[MAX_FILE];
	char name[MAX_STRING];
	char picture[MAX_PICTURE];
	int score;
	int count = 0;
	FILE* input;
	FILE* output;
	ART* art;
	ART* artPtr;

	//Statements
	strcpy(filename, prompt("Enter the name of the input file: "));
	input = openFile(filename, "r");
	art = createArray(MAX_ARR);
	artPtr = art;
	while(fgets(name, MAX_STRING, input))
	{
		fscanf(input, "%[^#]%*c%d%*c", picture, &score);
		saveToArt(name, picture, score, artPtr++);
		count++;
	}
	if (!(art = (ART*) realloc(art, count * sizeof(ART))))
		ERR_NO_MEMORY;
	if( fclose(input) == EOF )
		ERR_CLOSING_FILE;
	selectionSortAscending(art, count);
	strcpy(filename, prompt("Enter the name of the first output file: "));
	output = openFile(filename, "w");
	saveToFile(output, art, count);
	if( fclose(output) == EOF )
		ERR_CLOSING_FILE;
	strcpy(filename, prompt("Enter the name of the second output file: "));
	output = fopen(filename, "w");
	highestScore(output, art, count);
	if( fclose(output) == EOF )
		ERR_CLOSING_FILE;
	art = freeMemory(art, count);
	printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "\tNo Memory Leak\n");
	return 0;
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
	//Local Declarations
	FILE *input;

	//Statments
	if(!(input = fopen(filename, mode)))
		ERR_OPENING_FILE;

	return input;
}

//========================== allocateString ========================
/*This Function allocates memory for a string
PRE: stName
POST: memory has been allocated
Returns: a pointer to the allocated string
*/
char* allocateString (char stName[])
{
	//Local Declaration
	int len;
	char *pChar;

	//Statements
	len = strlen(stName);
	if (!(pChar = (char*) malloc ((len + 1)* sizeof(char))))
		ERR_NO_MEMORY;
	strcpy(pChar, stName);
	return pChar;

}

//========================== createArray ========================
/*This Function allocates memory an array of ART
PRE: size of the array
POST: memory has been allocated
Returns: a pointer to the allocated string
*/
ART* createArray (int size)
{
	ART* art;
	 
	if (!(art = (ART*) malloc (size * sizeof(ART))))
		ERR_NO_MEMORY;
	return art;
}

//========================== ReplaceN ========================
/*This Function replaces the \n at the end of an string with a \0
PRE: str
POST: \n has been replaced
Returns: nothing
*/
void replaceN (char str[])
{
	*(str + (strlen(str) - 1)) = '\0' ;

	return;
}

//========================== saveToArt ========================
/*This Function fills in the array of Art
PRE: name of the artist, the art, and the score of the artist
POST: the array has been filled in
Returns: nothing
*/
void saveToArt (char name[], char picture[], int score, ART* artPtr)
{
	replaceN(name);
	replaceN(picture);
	artPtr->artist = allocateString(name);
	artPtr->cartoon = allocateString(picture);
	artPtr->score = score;

	return;
}

//========================== print ========================
/*This Function prints an array of ART
PRE: array of Art, size of the array
POST: array is printed
Returns: nothing
*/
void print (ART* art, int size)
{
	
	for(ART* ptr = art; ptr < (art + size); ptr++)
		printf("%s\n%s\n# %d\n\n", ptr->artist, ptr->cartoon, ptr->score);

	return;
}

//========================== selectionSort ========================
/*This Function sorts the array in ascending order
PRE: array of ART, size of the array
POST: array has been sorted
Returns: nothing
*/
void selectionSortAscending( ART* myArr, int size )
{
	ART    temp;
	ART    *walker;
	ART    *smallest;  

	smallest = myArr;
	for (int wall = 0; wall < size - 1; wall++)   
	{
		walker = myArr + 1;
		for (int curr = wall + 1; curr < size; curr++){
			if ( strcmp(walker->artist ,smallest->artist) < 0)
				smallest = walker;
			walker++;
		}
		temp   = *myArr;
		*myArr = *smallest;
		*smallest = temp;
		myArr++;
	}	
    return ;
}

//========================== saveToFile ========================
/*This Function saves the output into a file
PRE: FILE*, array of ART, size of the array
POST: the output has been saved into a file
Returns: nothing
*/
void saveToFile(FILE* output, ART* art, int size)
{
	ART* pFirst = art;
	ART* pWalk = art + 1;
	int count;

	while(pFirst < (art + size)){
		count = 1;
		while(pWalk < (art + size) && !strcmp(pFirst->artist, (pWalk++)->artist))
			count++;
		fprintf(output, "%d %s\n", count, pFirst->artist);
		for(pWalk = pFirst; pWalk < (pFirst + count); pWalk++)
			fprintf(output, "%s\n# %d\n", pWalk->cartoon, pWalk->score);
		fprintf(output, "\n");
		pFirst = pFirst + count;
		pWalk = pFirst + 1;
	}
	return;
}

//========================== highestScore ========================
/*This Function finds the highest scores and saves them into a file
PRE: array of ART, size of the array
POST: the output has been saved into a file
Returns: nothing
*/
void highestScore(FILE* output, ART* art, int size)
{
	ART* pWalk;
	ART* temp = art;

	for(pWalk = (art + 1); pWalk < (art + size); pWalk++)
		if(temp->score < pWalk->score)
			temp = pWalk;
			
	fprintf(output, "%d is the highest score.\nThe following student(s) got the highest score:\n", temp->score);
	for(pWalk = art; pWalk < (art + size); pWalk++)
		if(temp->score == pWalk->score){
			fputs(pWalk->artist, output);
			fprintf(output, "\n");
			fputs(pWalk->cartoon, output);
			fprintf(output, "\n");
		}
	return;
}

//========================== prompt ========================
/*This Function prompts the user for a string
PRE: question in a char* type
POST: 
Returns: returns the user input
*/
char* prompt(char arr[])
{
	char str[MAX_FILE];

	printf("%s", arr);
	scanf("%s", &str);
	FLUSH;

	return str;
}

//========================== freeMemory ========================
/*This Function frees the momery
PRE: array of ART, size of the array
POST: memory is free
Returns: NULL
*/
ART* freeMemory(ART* art, int size)
{
	for(ART* pWalk = (art + size - 1); pWalk > art - 1; pWalk--){
		free(pWalk->artist);
		free(pWalk->cartoon);
	}
	free(art);
	return NULL;
}
/*
Screen output:

Enter the name of the input file: art.txt
Enter the name of the first output file: output1.txt
Enter the name of the second output file: output2.txt
        No Memory Leak
*/