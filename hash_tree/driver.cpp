#include "hash.h"
#define MEM_ERR   printf("  Memory overflow!!!\n"), exit(200)
#define ERR_OPENING_FILE  printf(" Error Opening File!!!\n"), getchar(), exit(102)
#define ERR_CLOSING_FILE  printf(" Error Closing File!!!\n"), getchar(), exit(103)

COUNTRY *allocateMemory (void);
void formatString (char *s);
COUNTRY *saveToNode (char code[], char name[], 
			char continent[], int population, double gdp);
char* allocateString (char stName[]);
FILE *openFile(int mode);
int  compareNames   (void* stu1, void* stu2);
void freeCountry(void* dataPtr);
int hash(char key[]);
int insertHash ( HASH_NODE* hashTable, COUNTRY* country);
HASH_NODE* buildHash(int size);
void printHash(HASH_NODE* hashTable);
int deleteHash(HASH_NODE* hashTable, char code[]);
void printTree (void* dataPtr);
HEADER* createHeader(int size);
void processManager (HEADER* pHead);
int findHashSize(void);
int countLines();
char getOption(void);
void printMenu (void);
void destroyHash(HEADER* header, int size);
HEADER* freeMemory (HEADER* header, int size);
int searchHash(HASH_NODE* pHash, char code[], COUNTRY** dataPtr);
COUNTRY* getAtribute (void);
void efficiency(HASH_NODE* pHash, int size);
int insertManager(HEADER* header, COUNTRY* ptr);
void insertData(HEADER* header);
void printCountry(COUNTRY* ptr);
void searchManager(HASH_NODE* pHash);
void deleteManager(HEADER* header);
static void _indentedTree (NODE* root, int tabNum);
void indentedTree (BST_TREE* tree);
void saveToFile(HEADER* header);

int main (void)
{
	FILE* input;
	COUNTRY* ptr;
	COUNTRY* foundCountry;
	HEADER *header;
	char code[4];
	char name[NAME_SIZE]; 
	char continent[NAME_SIZE];
	int population;
	double gdp;


	int size = findHashSize();
	header = createHeader(size);
	header->arrSize = size;		
	input = openFile(1);
	while( (fscanf(input, "%s %[^;] %*c %[^;] %*c %d %lf", 
						code, name, continent, &population, &gdp)) == 5) {
		ptr = saveToNode(code, name, continent, population, gdp);
		insertManager(header, ptr);	
	}
	processManager(header);
	header = freeMemory(header, size);
	printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
	system("pause");
	return 0;
}

/*	====================== compareNames ======================  
	Compare two student id's and return low, equal, high.
	    Pre  stu1 and stu2 are valid pointers to students
	    Post return low (-1), equal (0), or high (+1)
*/
int  compareNames   (void* stu1, void* stu2)
{
//	Local Definitions 
	COUNTRY s1 = *((COUNTRY*)stu1);
    COUNTRY s2 = *((COUNTRY*)stu2);;

//	Statements 
	return strcmp(s1.countryCode, s2.countryCode);
}

//===================== openInputFile ======================
/*
This function opens a file 
pre: nothing
post: opens the file
return: pointer to the file
*/
FILE *openFile(int mode)
{
	//Local Declarations
	char filename[FILENAME_SIZE];
	FILE *input;

	//Statments
	/*printf(" Enter the filename: ");
	scanf("%s", &filename);
	FLUSH;
	strcat(filename, ".txt");*/
	if (mode)
		input = fopen("a.txt", "r");
	else
		input = fopen("b.txt", "w");
	
	 
	if (input == NULL){
		printf( "%s does not exist.\n", filename );
		exit(101);
	}

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
	if ((pChar = (char*) malloc ((len + 1)* sizeof(char))) == NULL)
	{
		printf(" ERROR, not enough memory\n");
		exit (100);
	}
	strcpy(pChar, stName);
	return pChar;

}

//========================== saveToNode ========================
 /*This function saves the data file into a double link list
 PRE: code, airportName
 POST: node has been initialized
 Returns: pointer to the node
 */
COUNTRY *saveToNode (char code[], char name[], 
			char continent[], int population, double gdp)
{
	//Local Declarations
	COUNTRY *newNode;
	char *countryName;
	char *continentName;
		
	//Statments
	countryName = allocateString(name);
	continentName = allocateString(continent);
	newNode = allocateMemory();
	strcpy(newNode->countryCode, code);
	newNode->countryAttributes.countryName = countryName;
	newNode->countryAttributes.continent = continentName;
	newNode->countryAttributes.gdp = gdp;
	newNode->countryAttributes.population = population;
	
	return newNode;
}

//======================= allocateMemory ===================
/*This function allocates memory for a DNODE
PRE: nothing
POST: memery has been allocated
Rrturns: pointer to the allocated dnode
*/
COUNTRY *allocateMemory (void)
 {
	//Local Declarations
	COUNTRY *ptr;

	//Statments
	if ( !(ptr = ( COUNTRY* ) malloc ( sizeof (COUNTRY)))){
		printf( "Not enough memory\n" );
		exit(101);
	}
	else
		return ptr;
}

int hash(char key[])
{
	int looper;
	int addr   = 0;


	for (looper = 0 ;key[looper] != '\0'; looper++)
			addr = addr + key[looper];
	addr = addr % MAX_SIZE;
	return addr;
}
//size_t hash(char *s)
//{
//	static const size_t InitialFNV = 2166136261U;
//	static const size_t FNVMultiple = 16777619;
//    size_t hash = InitialFNV;
//
//    for(size_t i = 0; i < strlen(s); i++)
//    {
//        hash = hash ^ (s[i]);       /* xor  the low 8 bits */
//        hash = hash * FNVMultiple;  /* multiply by the magic number */
//    }
//    return hash % MAX_SIZE;
//}

int insertHash (HASH_NODE* hashTable, COUNTRY* country)
{
	int success;
	int index;

	index = hash(country->countryCode);
	if (hashTable[index].country != NULL)
	{
			if(hashTable[index].collision == NULL)
				hashTable[index].collision = createList(compareNames);
			success = addNode(hashTable[index].collision, country);
	}
	else{
		hashTable[index].country = country;
		success = 0;
	} 


	return success;
}

HASH_NODE* buildHash(int size)
{
	HASH_NODE* hashTable;

	
	if((hashTable = (HASH_NODE*) calloc ( MAX_SIZE, sizeof (HASH_NODE))) == NULL){
		printf( "Not enough memory\n" );
		exit(101);
	}
	for(int i = 0; i < size; i++){
		hashTable[i].country = NULL;
		hashTable[i].collision = NULL;
	}

	return hashTable;
}
//========================== CountLines ==========================
/* count lines of a file
Pre:  Nothing
Post: count file's lines and return this
*/
int countLines()
{
	FILE* fptr;
	char  input[81];
	int   count = 0;

	fptr = fopen("a.txt", "r");
		if(!fptr)
			ERR_OPENING_FILE;
	while((fgets(input, sizeof(input), fptr)))
		count++;
	if( fclose(fptr) == EOF )
		ERR_CLOSING_FILE;
	return count;
}
//=======================findHashSize======================
/*This function finds the first prime number greater than count by 2
 Pre:  Nothing
 Post: calculate prime number and return this as an index
*/
int findHashSize(void)
{ 
	int found = 0;
	int myIndex;
	int count;
	int prime = 1;
	int i;

	count = countLines();
	myIndex = (count  * 2);
	while (!found)
	{
		myIndex = myIndex + 1;
		//loop to find prime number
		for(i = 2; i <= count && prime == 1; i++)
		{
			if ((myIndex % i) == 0 )
				prime = 0;
		}//end

		prime = 1;
		if(i == count + 1)
			found = 1;
	}
	return myIndex ;
}
//======================
void printHash(HASH_NODE* hashTable)
{
	int index;
	void* collision;
	COUNTRY* country;

	for( index = 0; index < MAX_SIZE; index++){
		if(hashTable[index].country != NULL){
			printCountry(hashTable[index].country);
			if (hashTable[index].collision != NULL && hashTable[index].collision->count){
				for(int k = 0; k < hashTable[index].collision->count; k++){
					traverse(hashTable[index].collision, k, &collision);
					country = (COUNTRY*)collision;
					printCountry(country);
				}
			}
			printf("\n");
		}
	}

}

int deleteHash(HASH_NODE* hashTable, char code[])
{
	int index;
	int success = 0;
	void* ptr;
	COUNTRY* country;
	
	index = hash(code);
	if (hashTable[index].country != NULL)
	{
		if (strcmp(hashTable[index].country->countryCode, code) == 0){
			if(hashTable[index].collision != NULL){
				country = (COUNTRY*)hashTable[index].collision->head->dataPtr;
				success = removeNode(hashTable[index].collision, country->countryCode, &ptr);
				country = (COUNTRY*)ptr;
				hashTable[index].country = country;
			}
			else
				hashTable[index].country = NULL;	
			success = 1;
		}
		else
			if(hashTable[index].collision != NULL){
				success = removeNode(hashTable[index].collision, code, &ptr);
			}
	}
	else
		success = -1;

	return success;	
}

void freeCountry(void* dataPtr)
{
	COUNTRY* country = (COUNTRY*)dataPtr;

	free(country->countryAttributes.countryName);
	free(country->countryAttributes.continent);

	return;
}

void printTree (void* dataPtr)
{
	COUNTRY* country =  (COUNTRY*)dataPtr;
	printf("%s %s %s %d %6.2f\n", country->countryCode,
								   country->countryAttributes.countryName,
								   country->countryAttributes.continent, 
								   country->countryAttributes.population,
									country->countryAttributes.gdp );
	return;
}
/*====================== creatHeader =====================/*
/*Create a header to the main header structure
Pre:  Nothing
Post: A pointer to the main header
*/
HEADER* createHeader(int size)
{
	HEADER* pHead;
	
	if(!(pHead = (HEADER*) malloc (sizeof(HEADER))))
			MEM_ERR;
	pHead->count = 0;
	pHead->pTree = BST_Create(compareNames);
	pHead->pHash = buildHash(size); 
	pHead->arrSize = findHashSize( );
	
	return pHead;
}
/* ========================= processManager ====================
	Process user's option by calling appropriate functions.
	   PRE :   pHead - a pointer to the main header
	   POST:   pHead - might be changed after inserting or deleting 
       return: Nothing
*/
void processManager (HEADER* pHead)
{
	//Local Definitions 
	char option;

	//Statements 
	printMenu();
	do
	{	
		option = getOption();
		switch(option)
		{
		    case 0:  printMenu();
				       break;
			case 1:  insertData(pHead);
				       break;
			case 2:  deleteManager(pHead);
				       break;
		    case 3 : searchManager(pHead->pHash);
				       break;
		    case 4 : printHash(pHead->pHash);
				       break;
		    case 5 : BST_Traverse(pHead->pTree, printTree );
				       break;
		    case 6 : indentedTree(pHead->pTree);
				       break;
			case 7 : saveToFile(pHead);
					   break;	
		    case 8 : efficiency(pHead->pHash, pHead->arrSize);
				       break;		  
		    case 9 : printf("End of processing!\n");
                       break;
			default  :
				      break;
		} 
	} while (option != 9);
	return;
}// processListManager 

/* ============================== printMenu ==============================
	Prints the menu to the screen.
	PRE  : nothing
	POST : menu printed
*/
void printMenu (void)
{
//  Local Definitions 
	
//  Statements 
	printf("\n\t\t*********************************************"
			"\n\t\t*                                          *"
			"\n\t\t* 0  - Print menu                          *"
			"\n\t\t* 1  - Add new data                        *"
			"\n\t\t* 2  - Delete data                         *"
			"\n\t\t* 3  - Find                                *"
			"\n\t\t* 4  - List data in hash table sequence    *"            
			"\n\t\t* 5  - List data in key sequence(sorted)   *"
			"\n\t\t* 6  - Print indented tree                 *"
			"\n\t\t* 7  - Write data to a file                *"
			"\n\t\t* 8  - Efficiency                          *" 
			"\n\t\t* 9  - Quit                                *" 
			"\n\t\t*                                          *"
	        "\n\t\t********************************************");
	return;
} // printMenu 
/*	==================== getOption ====================
Gets and validates the user's option.
	Pre		: nothing
	Post	: valid option returned
*/   
char getOption(void)
{
//  Local Definitions 
	int option;

//  Statements 
	printf ("\n\n\tPlease enter the option between 0 to 9 (0 to print menu): ");
	scanf  (" %d", &option);
    FLUSH;
    while(option < 0 || option > 9)
    {
        printf("\t\a*** Invalid option! ***\n");
        printf("\tEnter one digit between 0 to 9: " );
        scanf  ("%d", &option);
        FLUSH;
    }
	return option;
} // getOption

void destroyHash(HEADER* header, int size)
{
	for(int i = 0; i <= size; i++){
		if(header->pHash[i].collision != NULL)
			header->pHash[i].collision = destroyList(header->pHash[i].collision);
	}
	return;
}

HEADER* freeMemory (HEADER* header, int size)
{
	destroyHash(header, size);
	BST_Destroy(header->pTree, freeCountry, 1);
	free(header->pHash);
	free(header);

	return NULL;
}

int searchHash(HASH_NODE* pHash, char code[], COUNTRY** data)
{
	int found = 0;
	void* dataPtr;

	int index = hash(code);
	if(pHash[index].country != NULL)
	{
		if(strcmp(pHash[index].country->countryCode, code) == 0){
			*data = pHash[index].country;
			found = 1;
		}
		else if(pHash[index].collision != NULL)
		{
			found = searchList(pHash[index].collision, code, &dataPtr);
			*data = (COUNTRY*)dataPtr;
		}
	}
	return found;
}

void efficiency(HASH_NODE* pHash, int size)
{
	int temp = 0;
	int total = 0;
	int counter = 0;

	for (int i = 0; i <= size; i++){
		if(pHash[i].country != NULL){
			counter++;
		}
		if(pHash[i].collision != NULL){
			total += pHash[i].collision->count;
			if(temp < pHash[i].collision->count)
				temp = pHash[i].collision->count;
		}
	}
	printf("Load factor: %%6.2lf\n", (counter/size)*100);
	printf("Number of collisions: %d\n", total);
	printf("Longest collision path: %d\n", temp);

	return; 
}

void insertData(HEADER* header)
{
	COUNTRY* ptr;
	
	ptr = getAtribute();
	if(insertManager(header, ptr))
		printf("\n\tThe country has been inserted successfully");
	else
		printf("\n\tThis country already exists");
		
	return;
}

COUNTRY* getAtribute (void)
{
	char name[30];
	char continent[30];
	int population;
	double gdp;
	char code[30];
	COUNTRY* ptr;

	do{
		printf("\n\tPlease enter the 3 letter country code: ");
		gets(code);
	}while(strlen(code) != 3);
	for(int i = 0; i < 3; i++)
		code[i] = toupper(code[i]);
	printf("\n\tEnter the name of the Country: ");
	gets(name);
	formatString(name);
	printf("\n\tEnter the name of the continent: ");
	gets(continent);
	formatString(continent);
	printf("\n\tEnter the population of the country: ");
	while(scanf("%d", &population) != 1){
		printf("\n\tPopulation is an integer, try again: ");
		FLUSH;
	}
	printf("\n\tEnter the GDP of the country: ");
	while(scanf("%lf", &gdp) != 1){
		printf("\n\tGdP is a double, try again: ");
		FLUSH;
	}
	ptr = saveToNode(code, name, continent, population, gdp);
	
	return ptr;
}

//==================== formatString =====================
/* This function converts the string to lower case excepet
	the first letter
	Pre: string
	POST: string fomated
	Return: nothing
*/
void formatString (char *s)
{
	if ( *s != '\0')
	{	
		*s = toupper(*s);
		s++;
		while(*s != '\0')
		{
			*s = tolower(*s);
			if(isalpha(*s) && *(s-1) == ' ')
				*s = toupper(*s);
			s++;
		}
	}
	return;
}

int insertManager(HEADER* header, COUNTRY* ptr)
{
	int success = 0;
	COUNTRY* country;

	if(!(searchHash(header->pHash, ptr->countryCode, &country)))
		{
			BST_Insert((header->pTree), ptr);		
			insertHash(header->pHash, ptr);	
			success = 1;
		}	
	return success;	
}

void searchManager(HASH_NODE* pHash)
{
	COUNTRY* ptr;
	char code[30];

	do{
		printf("\n\tPlease enter the 3 letter country code: ");
		gets(code);
	}while(strlen(code) != 3);
	for(int i = 0; i < 3; i++)
		code[i] = toupper(code[i]);

	if (searchHash(pHash, code, &ptr))
		printCountry(ptr);
	else
		printf("\n\tThis country does NOT exist: ");
	return;
}

void printCountry(COUNTRY* ptr)
{
	printf("\n\t%s %s %s %d %6.2lf", ptr->countryCode,
										ptr->countryAttributes.countryName,
										ptr->countryAttributes.continent,
										ptr->countryAttributes.population,
										ptr->countryAttributes.gdp);
}

void deleteManager(HEADER* header)
{
	char code[30];
	COUNTRY* country;
	COUNTRY* ptr;

	do{
		printf("\n\tPlease enter the 3 letter country code: ");
		gets(code);
	}while(strlen(code) != 3);
	for(int i = 0; i < 3; i++)
		code[i] = toupper(code[i]);

	if(searchHash(header->pHash, code, &ptr)){
		deleteHash(header->pHash, code);
		BST_Delete(header->pTree, ptr, freeCountry);
		printf("\n\tThe country has been deleted successfully");
	}
	else{
		printf("\n\tThe country that you want to delete does NOT exist");
	}

}

/*	=================== BST_Traverse =================== 
	Process tree using inorder traversal. 
	   Pre   Tree has been created (may be null) 
	         process ÒvisitsÓ nodes during traversal 
	   Post  Nodes processed in LNR (inorder) sequence 
*/
void indentedTree (BST_TREE* tree) 
{
	int tabNum = 0;
	_indentedTree (tree->root, tabNum);
	return;
} // end BST_Traverse 

/*	=================== _traverse =================== 
	Inorder tree traversal. To process a node, we use 
	the function passed when traversal was called.
	   Pre   Tree has been created (may be null) 
	   Post  All nodes processed 
*/
static void _indentedTree (NODE* root, int tabNum) 
{
    if (root){
        _indentedTree (root->left, tabNum + 1);
		for(int i = 0; i < tabNum; i++)
			printf("   ");
		printf("%d.%s\n", tabNum, ((COUNTRY*)root->dataPtr)->countryCode);
        _indentedTree (root->right, tabNum + 1);
    }
    return;
}// _traverse

void saveToFile(HEADER* header)
{
	FILE* output;
	int index;
	void* collision;
	COUNTRY* country;

	output = openFile(0);
	for( index = 0; index < header->arrSize; index++){
		if(header->pHash[index].country != NULL){
			fprintf(output, "%s %s; %s; %d %6.2lf\n", header->pHash[index].country->countryCode,
												header->pHash[index].country->countryAttributes.countryName,
												header->pHash[index].country->countryAttributes.continent,
												header->pHash[index].country->countryAttributes.population,
												header->pHash[index].country->countryAttributes.gdp);
			if (header->pHash[index].collision != NULL && header->pHash[index].collision->count){
				for(int k = 0; k < header->pHash[index].collision->count; k++){
					traverse(header->pHash[index].collision, k, &collision);
					country = (COUNTRY*)collision;
					fprintf(output, "%s %s; %s; %d %6.2lf\n", country->countryCode,
															country->countryAttributes.countryName,
															country->countryAttributes.continent,
															country->countryAttributes.population,
															country->countryAttributes.gdp);
				}
			}
		}
	}
	printf("\n\tAll datas have been saved to a file");
	if( fclose(output) == EOF )
		ERR_CLOSING_FILE;
}