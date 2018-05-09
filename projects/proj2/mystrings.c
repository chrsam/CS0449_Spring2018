// Chris Mananghaya (cam314)
// CS 0449: Project 2

/*	Goal: Create a program that can recover the password from three executable files.
 *	No source code is given, only the complied executables.
 *	The programs may have a single, fixed password, or they may use some kind of algorithm to generate/check the password. 
 *	The program to create, "mystrings.c", will need to extract readable strings from a binary file. 
 *	This will be based on a UNIX program called "strings", while lets you seach for strings within a binary file. 
 *
 *	Testing it: automated comparison
 *	$ strings -a -n 4 somefile > strings.out
 *	$ mystrings somefile > mystrings.out
 *	$ diff strings.out mystrings.out
 */ 

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	
	int length = 0;
	char next;
	char string[100];
	
	FILE *binary_file;
	binary_file = fopen(argv[1], "rb");

	if(argc != 2) {	// Checks for correct # of arguments. 
		printf("Error: incorrect number of arguments.\n");
		return 1;
	}

	if(binary_file == NULL) {		// Checks if file exists.
		printf("Error reading file. File may not exist.\n");
		return 0;
	}

	while(feof(binary_file) == 0) {
		fread(&next, sizeof(next), 1, binary_file);

		if(next >= 32 && next <= 126) {
			string[length] = next;
			length++;
			continue;
		}

		string[length] = '\0';

		if(length >= 4) {
			printf("%s\n", string);
		}

		length = 0;
		string[0] = '\0';
	}

	fclose(binary_file);
	// return 0;
}

