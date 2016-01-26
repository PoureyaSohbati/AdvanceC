/*This program reads data from a file and 
	hashes it and turns it into a binary file.
Name: Poureya Sohbati
Date: 5/15/14
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
	char firstname[21];
	char lastname[21];
	char amount[9];
	char student_id[5];
}NODE;

#define SIZE 50
#define BUCKET_SIZE 3
#define FLUSH while (getchar () != '\n')
#define ERR_NO_MEMORY     printf(" ERROR, not enough memory\n"), getchar(), exit (100)
#define ERR_OPENING_FILE  printf(" Error Opening File!!!\n"), getchar(), exit(102)
#define ERR_CLOSING_FILE  printf(" Error Closing File!!!\n"), getchar(), exit(103)

FILE* openFile(char filename[], char mode[]);
int searchBinary(FILE* fp, char *target);
int hash ( char *key, int size);
int insertBinary(FILE* fp, NODE data);
void deleteBinary(FILE* fp, char* target);
void printhash(FILE* fp);
int storeInNode (char* data, NODE* node);
char* checkArgs( int argc, char **argv);
void menu(void);
void findAndDisplay(FILE* output, char *id);
int numberOfParameters(char string[]);
char* getId(void);
void upperCase(char *string);
char getOption(void);

int main (int argc, char *argv[])
{
	FILE* input;
	FILE* output;
	char filename[15];
	NODE hashTable[50] = {""};
	char id[5];
	char option;
	int key;
	NODE data;
	char buffer[50];
	int valid;

	strcpy(filename, checkArgs(argc, argv));
	output = openFile("output.txt", "w+b");
	fwrite(hashTable, sizeof(NODE), SIZE, output); 
	input = openFile(filename, "r");
	while(fscanf(input, " %s %s %s %s", &data.student_id, data.lastname, data.firstname, &data.amount) == 4)
		if(!insertBinary(output, data))
			printf("\tCould not insert %s because the bucket is full for that key\n", data.student_id);
	if(!fclose(input) == EOF)
		ERR_CLOSING_FILE;

	menu();
	while(option = getOption(), option != 'Q')
	{
		switch(option)
		{
			case 'M': menu();
				break;
			case 'P': printhash(output);
				break;
			case 'A': printf("\tEnter the input(Id Lastname Firstname Amount): ");
					  gets(buffer);
					  if(storeInNode (buffer, &data)){
						  if(valid = insertBinary(output, data))
							 printf("\tThe data was added successfully\n");
						  else if(valid == -1)
							 printf("\tCould not insert %s because the bucket is "
									"full for that key\n", data.student_id);
					  }
					  break;
			case 'D': strcpy(id, getId());
					  deleteBinary(output, id);
					  break;
			case 'S': strcpy(id, getId());
					  findAndDisplay(output, id);
					  break;					
		}		
	}

	if(!fclose(output) == EOF)
		ERR_CLOSING_FILE;
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
	FILE *file;

	if(!(file = fopen(filename, mode)))
		ERR_OPENING_FILE;

	return file;
}

//int hash(char* id, int size){
//	
//	int temp = 0;
//
//	for(int i = 0; i < 4; i++)
//		temp += pow((id[i] - '0'), 2.0);
//
//	return (int) (temp % size);
//}

//===================== hash  ======================
/*
This function hashes the array of chars
PRE: char*, size of the hash table
POST: finds the key
Return: int hash
*/
int hash(char *str, int size)
{
	int hash = 53811431234;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; // (shifts hash 5 bits to the left) + hash + c
	if (hash < 0)
		hash = abs(hash);
	hash %= size;

   return hash;
}

//===================== insertBinary ======================
/*
This function inserts the data into a hash table on the disk
PRE: pointer to file, data
POST: inserts the data into a binary file
Return: 1 if it was added successfully, 
		0 if data already exists, -1 if the bucket is full
*/
int insertBinary(FILE* fp, NODE data)
{

	int index;
	int counter = 0;
	NODE temp;

	if(searchBinary(fp, data.student_id)){
		printf("\tThis data already exists\n");
		return 0;
	}

	else
	{
		index = hash(data.student_id, SIZE);
		index = (index / BUCKET_SIZE) * BUCKET_SIZE;
		fseek(fp, index * sizeof(NODE), SEEK_SET);

		while(fread(&temp, sizeof(NODE), 1, fp), temp.firstname[0] && counter < BUCKET_SIZE)
			counter++;
	
		if(temp.firstname[0])
			return -1;

		else{
			fseek(fp, -1 * sizeof(NODE), SEEK_CUR);
			fwrite(&data, sizeof(NODE), 1, fp);
			return 1;
		}
	}
}

//===================== deleteBinary ======================
/*
This function deletes data from the hash table in the file
PRE: pointer to the file and id number
POST: data has been deleted
Return: nothhing
*/
void deleteBinary(FILE* fp, char* target)
{
	NODE temp;
	int i;
	int last;

	if(searchBinary(fp, target))
	{
		fseek(fp, -1 * sizeof(NODE), SEEK_CUR);
		i = ftell(fp)/sizeof(NODE);
		last = (((i/BUCKET_SIZE) + 1) * BUCKET_SIZE) - 1;

		while((i = ftell(fp)/sizeof(NODE)) <= last)
		{
			if(i == last)						// last index in the bucket
			{
				strcpy(temp.student_id, "");
				strcpy(temp.firstname, "");
				strcpy(temp.lastname, "");
				strcpy(temp.amount, "");
				fwrite(&temp, sizeof(NODE), 1, fp);
			}
			else
			{
				fseek(fp, 1 * sizeof(NODE), SEEK_CUR);
				fread(&temp, sizeof(NODE), 1, fp);
				fseek(fp, -2 * sizeof(NODE), SEEK_CUR);
				fwrite(&temp, sizeof(NODE), 1, fp);
			}
		}
		printf("\tThe student %s was deleted from the hash.\n", target);
	}
	else
		printf("\tThe student %s does not exis.\n", target);

	return;
}

//===================== searchBinary ======================
/*
This function searches for a student using the student id
PRE: pointer to the file, id
POST: finds the student 
Return: 1 if the student was found and 0 if does not exists
*/
int searchBinary(FILE* fp, char *target)
{
	int address;
	NODE temp;
	int counter = 0;
	int found = 1;

	address = hash(target, SIZE);
	address = (address / BUCKET_SIZE) * BUCKET_SIZE;
	fseek(fp, address * sizeof (NODE), SEEK_SET);
	while(fread(&temp, sizeof (NODE), 1, fp), temp.firstname[0] 
			&& (found = strcmp(temp.student_id, target)) && counter < BUCKET_SIZE)
		counter++;

	return !found ? 1 : 0;   
}

//===================== printHash ======================
/*
This function prints the hash table from the binary file
PRE: pointer to file
POST: prints the hash table
Return: nothing
*/
void printhash(FILE* fp)
{
	NODE data;
	int i = 0;

	rewind(fp);
	while(fread(&data, sizeof(NODE), 1, fp))
		printf("\t%02d: %s %s %s %s\n", i++, data.student_id, data.lastname,
											 data.firstname, data.amount);
	return;
}

//===================== storeInNode ======================
/*
This function stores the data into a node
PRE: line of data(char*), node
POST: adds the data into a node
Return: 0 if the user input is wrong and 1 if the data was acceptable
*/
int storeInNode (char* data, NODE* node)
{
	char* ptr;
	int len;
	int num = 0;
	int counter = 0;
	char *digits = "0123456789";
	
	if(numberOfParameters(data) == 4)
	{
		ptr = strtok(data, " ");
		len = strlen(ptr);
		if (len != 4 || len != strspn(ptr, digits)){
			printf("\tWrong Input! The student id should be a four digit number.\n");
			return 0;
		}

		strcpy(node->student_id, ptr);

		ptr = strtok(NULL, " ");
		len = strlen(ptr);
		if (len > 20 || len != strcspn(ptr, digits)){
			printf("\tWrong Input! The lastname of the student should be\n\t\ta 20" 
					" characters long from A to Z(not case sensetive).\n");
			return 0;
		}

		upperCase(ptr);
		strcpy(node->lastname, ptr);

		ptr = strtok(NULL, " ");
		len = strlen(ptr);
		if (len > 20 || len != strcspn(ptr, digits)){
			printf("\tWrong Input! The firstname of the student should be\n\t\ta 20"
					" characters long from A to Z(not case sensetive).\n");
			return 0;
		}

		upperCase(ptr);
		strcpy(node->firstname, ptr);

		ptr = strtok(NULL, " ");
		num = strspn(ptr, digits);
		if(num < 2 || num > 5 || strspn(ptr+num+1, digits) != 2){
			printf("\tWrong Input! The amount should be a number between 10.00 and max = 99999.99.\n");
			return 0;
		}
	}
	else
	{
		printf("\tWrong number of parameters. There should be four parameters.\n");
		return 0;
	}

	return 1;

}

//===================== checkArgs ======================
/*
This function validates the arguments that the user has entered 
PRE: number of arguments, array of char*
POST: checks the user input
Return: the name of the file
*/
char* checkArgs( int argc, char **argv)
{

    if (argc > 2){
		printf("\tWrong number of parameters. There should be four parameters.(Only the name of the file)\n"); 
		getchar();
		exit(2);
	}
	return (argc == 2) ? argv[1] : "input.txt";
}

//===================== menu ======================
/*
This function prints the menu
PRE: nothing
POST: prints the menu
Return: nothing
*/
void menu(void)
{
	printf("\t***************************************************\n"
			"\t* M: Print Menu                                   *\n"
			"\t* P: Print hash                                   *\n"
			"\t* A: Add a student                                *\n"
			"\t* D: Delete a student                             *\n"
			"\t* S: Search a student                             *\n"
			"\t* Q: Quit                                         *\n"
			"\t***************************************************\n");
	return;
}

//===================== findAndDisplay ======================
/*
This function find the student and prints it on the screen
PRE: pointer to file, student id
POST: prints the student info on the screen
Return: nothing
*/
void findAndDisplay(FILE* output, char *id)
{

	NODE temp;


	if(searchBinary(output, id)){
		fseek(output, -1 * sizeof(NODE), SEEK_CUR);
		fread(&temp, sizeof(NODE), 1, output);
		printf("\t%s %s %s %s\n", temp.student_id, temp.lastname, temp.firstname, temp.amount);
	}
	else
		printf("\tStudent with the student id of %s does not exist.\n", id);
	return;

}

//===================== numberOfParameters ======================
/*
This function counts the number of the parameters that the user has entered
PRE: array of chars
POST: finds the number of parameters
Return: returns the number of parameters
*/
int numberOfParameters(char string[])
{
	int flag = 0;
	int num = 0;
	int len;

	len = strlen(string);
	for(int i = 0; i < len; i++){
		if (string[i] != ' ' && !flag)
			num++;
		string[i] == ' ' ? flag = 0 : flag = 1;
	}	 

	return num;
}

//===================== getId ======================
/*
This function gets the student id
PRE: nothing
POST: gets the student id
Return: student id
*/
char* getId(void)
{
	char id[20];
	NODE temp;
	int len;
	int flag = 0;

	while(!flag){
		printf("\tEnter the student id: ");
		scanf(" %s", id);
		FLUSH;
		len = strlen(id);
		if(len != 4 || len != strspn(id, "0123456789"))
			printf("\tWrong Input! Student Id should be a 4 digit number.\n");
		else
			flag = 1;
	}
	return id;
}

//===================== upperCase ======================
/*
This function turns an string into upper case
PRE: array of chars
POST: capitlizes the string
Return: nothing
*/
void upperCase(char *string)
{
	for(char *ptr = string; *ptr; ptr++)
		*ptr = toupper(*ptr);
	return;
}

//===================== getOption ======================
/*
This function gets the user input and validates it
PRE: nothing
POST: gets the input
Return: a character
*/
char getOption(void)
{
	char buffer[50];
	int flag = 0;

	while(!flag)
	{
		printf("\tEnter an option: ");
		gets(buffer);
		upperCase(buffer);
		if (strlen(buffer) != 1 || !strspn(buffer, "MPADSQ"))
			printf("\tWrong Input!\n\tThe input should be one"
					" of the following characters M, P, A, D, S, Q\n");
		else
			flag = 1;

	}
	return buffer[0];
}

/* Output:

        ***************************************************
        * M: Print Menu                                   *
        * P: Print hash                                   *
        * A: Add a student                                *
        * D: Delete a student                             *
        * S: Search a student                             *
        * Q: Quit                                         *
        ***************************************************
        Enter an option: p
        00:
        01:
        02:
        03:
        04:
        05:
        06: 6745 SMITH ANNA 7769.87
        07:
        08:
        09: 7146 MILLER ALISON 8934.12
        10: 9345 ROBINSON ANDRE 15.67
        11: 6275 ALLEN RAY 958.34
        12: 1622 MOORE TONI 83.65
        13: 1623 CLARK VICTOR 83.45
        14:
        15:
        16:
        17:
        18:
        19:
        20:
        21:
        22:
        23:
        24: 8624 BROWN YELENA 56.75
        25: 1832 TAYLOR JOAN 293.18
        26: 5392 HERNANDEZ MICHAEL 23.45
        27: 5675 JOHNSON SHEILA 23.91
        28: 5673 HALL MARC 93.13
        29:
        30:
        31:
        32:
        33: 4717 MARTINEZ JHON 85.11
        34:
        35:
        36:
        37:
        38:
        39: 2328 WILSON ROYCE 123.09
        40:
        41:
        42: 1235 WILLIAMS JANE 93.12
        43:
        44:
        45: 9162 DAVIS SUSAN 902.34
        46: 3271 GARCIA ROBERT 43.72
        47:
        48: 2341 JONES BARBARA 74.23
        49:
        Enter an option: a
        Enter the input(Id Lastname Firstname Amount): 2344 sohbati pourya 23.4
        Wrong Input! The amount should be a number between 10.00 and max = 99999.99.
        Enter an option: a
        Enter the input(Id Lastname Firstname Amount): 2344 pourya sohbati 23.43

        The data was added successfully
        Enter an option: s
        Enter the student id: 2344
        2344 POURYA SOHBATI 23.45
        Enter an option: d
        Enter the student id: 1235
        The student 1235 was deleted from the hash.
        Enter an option: s
        Enter the student id: 1235
        Student with the student id of 1235 does not exist.
        Enter an option: s
        Enter the student id: 2344
        2344 POURYA SOHBATI 23.45
        Enter an option: m
        ***************************************************
        * M: Print Menu                                   *
        * P: Print hash                                   *
        * A: Add a student                                *
        * D: Delete a student                             *
        * S: Search a student                             *
        * Q: Quit                                         *
        ***************************************************
        Enter an option: p
        00: 2344 POURYA SOHBATI 23.45
        01:
        02:
        03:
        04:
        05:
        06: 6745 SMITH ANNA 7769.87
        07:
        08:
        09: 7146 MILLER ALISON 8934.12
        10: 9345 ROBINSON ANDRE 15.67
        11: 6275 ALLEN RAY 958.34
        12: 1622 MOORE TONI 83.65
        13: 1623 CLARK VICTOR 83.45
        14:
        15:
        16:
        17:
        18:
        19:
        20:
        21:
        22:
        23:
        24: 8624 BROWN YELENA 56.75
        25: 1832 TAYLOR JOAN 293.18
        26: 5392 HERNANDEZ MICHAEL 23.45
        27: 5675 JOHNSON SHEILA 23.91
        28: 5673 HALL MARC 93.13
        29:
        30:
        31:
        32:
        33: 4717 MARTINEZ JHON 85.11
        34:
        35:
        36:
        37:
        38:
        39: 2328 WILSON ROYCE 123.09
        40:
        41:
        42:
        43:
        44:
        45: 9162 DAVIS SUSAN 902.34
        46: 3271 GARCIA ROBERT 43.72
        47:
        48: 2341 JONES BARBARA 74.23
        49:
        Enter an option: adfd
        Wrong Input!
        The input should be one of the following characters M, P, A, D, S, Q
        Enter an option: q
*/