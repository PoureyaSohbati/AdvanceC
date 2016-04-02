#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>
#include "linkListADT.h"
#include "bstADT.h"
//#include "queueADT.h"
#define MAX_SIZE 25
#define FILENAME_SIZE 30
#define FLUSH  while(getchar() != '\n')
#define NAME_SIZE 30


typedef struct{
	char* countryName;
	int population;
	char* continent;
	double gdp;
}ATTRIBUTE;

typedef struct
{
	char countryCode[4];
	ATTRIBUTE countryAttributes;

}COUNTRY;

typedef struct 
{
	COUNTRY *country;
	LIST* collision;

}HASH_NODE;


typedef struct
{
	int count;
	int arrSize;
    HASH_NODE* pHash;
	BST_TREE*  pTree;

}HEADER;