/*	
	This program turns on the light of a theater by doing bit manipulation
	Name: Poureya Sohbati
	Date: 5/29/14
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void printMenu(void);
void printBits(unsigned short lights);
int checkChioce(char *string);
void  turnOff(int numbits, int startbit);
void  turnOn(int numbits, int startbit);
int checkInput(char* str, int* numbits, long* startBit, unsigned short *pattern);
void bitPattern(void);


int main (void)
{
	char input[50];
	int option;

	printMenu();
	while(printf("\n\tEnter an option: "), gets(input), (option = checkChioce(input)) != 10)
		if(option)
		{
			switch(option)
			{
				case 1 : turnOn(16, 0);
					 break;
				case 2 : turnOn(6, 5);
						 break;
				case 3 : turnOn(5, 11);
						 break;
				case 4 : turnOn(5, 0);
						 break;
				case 5 : turnOff(16, 0);
						 break;
				case 6 : turnOff(6, 5);
						 break;
				case 7 : turnOff(5, 11);
						 break;
				case 8 : turnOff(5, 0);
						 break;
				case 9 : bitPattern();
						 break;
			}
		}
			
	return 0;
}

//************************** printMenu *************************
/*This function print a menu
Recieves: nothing
Returns: nothing
*/
void printMenu(void)
{
	printf("\n\t************************************************************\n"
			"\t* 1)  turn on all lights                                   *\n"
			"\t* 2)  turn on center stage lights (lights 5-10)            *\n"
			"\t* 3)  turn on left stage lights (lights 11-15)             *\n"
			"\t* 4)  turn on right stage lights (lights 0-4)              *\n"
			"\t* 5)  turn off all lights                                  *\n"
			"\t* 6)  turn off center stage lights                         *\n"
			"\t* 7)  turn off left stage lights                           *\n"
			"\t* 8)  turn off right stage lights                          *\n"
			"\t* 9)  enter on/off pattern                                 *\n"
			"\t* 10) quit                                                 *\n"
			"\t************************************************************\n");
	return;	
}

//************************** printBits *************************
/*This function prints the bits in an unsigned short
Recieves: unsigned short
Returns: nothing
*/
void printBits(unsigned short lights)
{
    printf("\n\tLights:\t");
	for(int i = 15; i >= 0;  i--)
	{	
		printf ("%ld", (lights >> i) & 1);
		if (!(i % 4))
			printf("  ");
	}
	printf("\n");
	return;
}

//************************** checkChoice *************************
/*This function validates a string
Recieves: char*
Returns: -1 if the string in empty
		0 if the the string is not valid
		an integer between 0 to 10
*/
int checkChioce(char *string)
{
	char *ptr;
	char *end;
	int num;

	if(!(ptr = strtok(string, "\040\t")))
	{
		printMenu();
		return -1;
	}
	if(strtok(NULL, "\040\t"))
	{
		printf("\tWrong number of parameters!Try again!\n");
		return 0;
	}
	else{
		num = strtol(ptr, &end, 10);
		if( *end != '\0')
		{
			printf("\tWrong input!Try again!\n");
			return 0;
		}
		else if(num < 1 || num > 10 ){
            printf("\tWrong input!The input must be between 1 and 10.\n");
			return 0;
		}
	}	  
	return num;
}

//************************** turnOFF *************************
/*This function turns oof the bits of an unsigned short
Recieves: number of bits that need to turn off and where it starts from
Returns: nothing
*/
void  turnOff(int numbits, int startbit)
{
	unsigned short lights = ~0;

	lights &= ~ (~((unsigned short) ~ 0 << numbits) << startbit);
	printBits(lights);
	return;
}

//************************** turnOn *************************
/*This function turns on the bits on an unsigned short
Recieves: number of bits that need to turn on and where it starts from
Returns: nothing
*/
void  turnOn(int numbits, int startbit)
{
	unsigned short lights = 0;

	lights |= (~((unsigned short) ~ 0 << numbits) << startbit);
	printBits(lights);
	return;
}

//************************** bitPattern *************************
/*This function does the given bit pattern
Recieves: nothing
Returns: nothing
*/
void bitPattern(void)
{
	unsigned short lights = ~0;
	char str[80];
	long startBit;
	int numbits;
	unsigned short pattern;

	while(printf("\tEnter starting light number and on/off pattern: "),
		        gets(str),
				!checkInput (str, &numbits, &startBit, &pattern));
	lights &= ~(~((unsigned short) ~0 << numbits) << startBit);
	lights |= pattern << startBit;
	printBits(lights);
	return;
}

//************************** checkInput *************************
/*This function checks the input for the option 9 in the menu
Recieves: string, pointer to number of bits, starting bit, and the pattern
Returns: 0 if not valid else 1
*/
int checkInput(char* str, int* numbits, long* startBit, unsigned short *pattern)
{
	char *end;
	char *token1;
	char *token2;

    // 1st token
	if(!(token1 = strtok(str, "\040\t")) ){
		printf("\tYou did not enter the any input! Try again!\n");
		return 0;
	}
	*pattern = (unsigned short) strtol(token1, &end, 2); 
	if(*end){
		printf("\tWrong on/off pattern! Try again!\n"); 
		return 0;
	}
	// 2nd token
	if(!(token2 = strtok(NULL, "\040\t")) ){
		printf("\tYou did not enter the starting bit! Try again!\n");
		return 0;
	}
	*startBit = strtol(token2, &end, 10);
	if(*end){
		printf("\tWrong Input!, Try again!\n"); 
		return 0;
	}
	if(*startBit < 0 || *startBit > 15  ) {
		printf("\tStarting light should be between 0 and 15! Try again!\n"); 
		return 0;
	}
	// 3th token
	if((strtok(NULL, "\040\t")) ){
		printf("\tWrong number of parameters! Try again!!\n"); 
		return 0;
	}
	*numbits = strlen(token1);
	if(*startBit + *numbits > 16){
		printf("\tYour pattern exceeds 16 bits! Try again!\n"); 
		return 0;
	}	
	return 1;
}
/*Output:

        ************************************************************
        * 1)  turn on all lights                                   *
        * 2)  turn on center stage lights (lights 5-10)            *
        * 3)  turn on left stage lights (lights 11-15)             *
        * 4)  turn on right stage lights (lights 0-4)              *
        * 5)  turn off all lights                                  *
        * 6)  turn off center stage lights                         *
        * 7)  turn off left stage lights                           *
        * 8)  turn off right stage lights                          *
        * 9)  enter on/off pattern                                 *
        * 10) quit                                                 *
        ************************************************************

        Enter an option: 1

        Lights: 1111  1111  1111  1111

        Enter an option: 2

        Lights: 0000  0111  1110  0000

        Enter an option:

        ************************************************************
        * 1)  turn on all lights                                   *
        * 2)  turn on center stage lights (lights 5-10)            *
        * 3)  turn on left stage lights (lights 11-15)             *
        * 4)  turn on right stage lights (lights 0-4)              *
        * 5)  turn off all lights                                  *
        * 6)  turn off center stage lights                         *
        * 7)  turn off left stage lights                           *
        * 8)  turn off right stage lights                          *
        * 9)  enter on/off pattern                                 *
        * 10) quit                                                 *
        ************************************************************

        Enter an option: asdf
        Wrong input!Try again!

        Enter an option: 3

        Lights: 1111  1000  0000  0000

        Enter an option: 4

        Lights: 0000  0000  0001  1111

        Enter an option: 234
        Wrong input!The input must be between 1 and 10.

        Enter an option: 5

        Lights: 0000  0000  0000  0000

        Enter an option: 6

        Lights: 1111  1000  0001  1111

        Enter an option:

        ************************************************************
        * 1)  turn on all lights                                   *
        * 2)  turn on center stage lights (lights 5-10)            *
        * 3)  turn on left stage lights (lights 11-15)             *
        * 4)  turn on right stage lights (lights 0-4)              *
        * 5)  turn off all lights                                  *
        * 6)  turn off center stage lights                         *
        * 7)  turn off left stage lights                           *
        * 8)  turn off right stage lights                          *
        * 9)  enter on/off pattern                                 *
        * 10) quit                                                 *
        ************************************************************

        Enter an option: 7

        Lights: 0000  0111  1111  1111

        Enter an option: 8

        Lights: 1111  1111  1110  0000

        Enter an option: 9
        Enter starting light number and on/off pattern: asdk
        Wrong on/off pattern! Try again!
        Enter starting light number and on/off pattern: 1110101 4

        Lights: 1111  1111  0101  1111

        Enter an option: 9
        Enter starting light number and on/off pattern: 1101 15
        Your pattern exceeds 16 bits! Try again!
        Enter starting light number and on/off pattern: 1102 afad
        Wrong on/off pattern! Try again!
        Enter starting light number and on/off pattern: 11011 asdf
        Wrong Input!, Try again!
        Enter starting light number and on/off pattern: 110100 2 324as
        Wrong number of parameters! Try again!!
        Enter starting light number and on/off pattern: 1100101 4

        Lights: 1111  1110  0101  1111

        Enter an option: 12
        Wrong input!The input must be between 1 and 10.

        Enter an option: 10	
*/