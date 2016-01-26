/*This program opens the two file (one input file and one output file)
	and prints the all of the identifiers in the input file into the second file
Name: Poureya Sohbati
Date: 6/17/2014
*/
#include "homework5.h"

FILE* openFile(char filename[], char mode[]);
void checkArgs( int argc, char **argv);
int compare(void* stu1, void* stu2);
int validation(char* str);
void comment_Quotation(char str[]);
void printLine(FILE* output, FILE* input, int arr[]);

int main (int argc, char *argv[])
{
	FILE* input;
	FILE* output;
	char buffer[100];
	HEADER* header;
	NODE*nodePtr;
	int line = 1;
	int arr[200] = {0};
	char *ptr;
	int flag = 0;
	char exceptions[35] = " ()*&[]!=/,-;+'\"{}@#$^\t<>\012|\t?:"; 

	checkArgs(argc, argv);
	header = (HEADER*) calloc (1, sizeof(HEADER));

	input = openFile(argv[1], "r");
	output = openFile(argv[2], "w");
	while(fgets(buffer, 100, input))
	{	
		arr[line] = ftell(input);
		if(strstr(buffer, "/*"))
			flag = 1;
		if(!flag){
			comment_Quotation(buffer);
			ptr = strtok(buffer, exceptions);
			if(ptr && validation(ptr))
			{
				insert(&header->root, ptr, line, compare);
				while(ptr = strtok(NULL, exceptions))
				{
					if(validation(ptr))
						insert(&header->root, ptr, line, compare);
				}
			}
		}
		if(flag && strstr(buffer, "*/"))
			flag = 0;
		line++;
	}
	arr[line-1] = 0;
	fprintf(output, "/************************  Cross Reference Listing"
					"  ************************/\n\n");
	print(output, header->root);
	printLine(output, input, arr);
	if( fclose(input) == EOF )
		ERR_CLOSING_FILE;
	if( fclose(output) == EOF )
		ERR_CLOSING_FILE;
	
	header->root = destroy(header->root);
	free(header);
	printf( _CrtDumpMemoryLeaks() ? "\n\tMemory Leak\n" : "\n\tNo Memory Leak\n");

	return 0;
}

//===================== openInputFile ======================
/*
This function opens a file 
PRE: filename, opening mode
POST: opens the file
*/
FILE* openFile(char filename[], char mode[])
{
	FILE *file;

	if(!(file = fopen(filename, mode)))
		ERR_OPENING_FILE;

	return file;
}

//===================== checkArgs ======================
/*
This function validates the arguments that the user has entered 
PRE: number of arguments, array of char*
POST: checks the user input
*/
void checkArgs( int argc, char **argv)
{

    if (argc != 3){
		printf("\tWrong number of parameters."
				"\n\tThere should be four parameters.(Only the name of the file)\n"); 
		getchar();
		exit(2);
	}
	return;
}

//===================== printLine ======================
/*
This function prints the the file into another gile
		 using the array of  offsets
PRE: printer to input and output file and array of offsets
POST: prints the file
*/
void printLine(FILE* output, FILE* input, int arr[])
{
	int *ptr = arr;
	char buffer[100];
	int lineNum = 1;

	fprintf(output, "\n\n\n");
	while(*ptr || lineNum == 1){
		fseek(input, *ptr, SEEK_SET);
		fgets(buffer, 100, input); 
		fprintf(output, "%-6d: %s", lineNum, buffer);
		lineNum++;
		ptr++;
	}
	return;
}

//===================== compare ======================
/*
This function this function compares two strings 
PRE: 2 void pointers
POST: returns the copare value
*/
int compare(void* stu1, void* stu2)
{
//	Local Definitions 
	char *s1 = ((char*)stu1);
    char *s2 = ((char*)stu2);;

//	Statements 
	return strcmp(s1, s2);
}

//===================== validation ======================
/*
This function validates the arguments that the user has entered 
PRE: a string
POST: returns 1 if valid 0 if not
*/
int validation(char* str)
{
	char digits[14] = "0123456789.\134%";
	if(!strcspn(str, digits))
		return 0;
	return 1;
}

//===================== comment_Quotation ======================
/*
This function gets rid of the comments and quotations 
PRE: array of chars
POST: gets rid of the comments and quotations
*/
void comment_Quotation(char str[])
{
	char *ptr;
	char temp[100];

	if((ptr = strchr(str, '/')) && (*(ptr+1) == '/'))	
		*ptr = '\0';

	while(ptr = strchr(str, '\"'))
	{
		strncpy(temp, str, ptr-str);
		temp[ptr-str] = '\0';
		if(ptr = strchr(ptr+1, '\"'))
			strcat(temp, ptr+1);
		strcpy(str, temp);
	}
	while(ptr = strchr(str, '\'')){
		strncpy(temp, str, ptr-str);
		temp[ptr-str] = '\0';
		if(ptr = strchr(ptr+1, '\''))
			strcat(temp, ptr+1);
		strcpy(str, temp);
	}
	return;
}