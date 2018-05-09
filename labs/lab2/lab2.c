// Chris Mananghaya (cam314)
// CS0449: Lab 2

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// prints a message, reads a line of input, and chops off the newline.
// call it like
// 		read_line("Type something in: ", buffer, sizeof(buffer));
void read_line(const char* message, char* buffer, int length)
{
	printf(message);
	fgets(buffer, length, stdin);
	if(strlen(buffer) != 0)
		buffer[strlen(buffer) - 1] = 0;
}

// sees if two strings are the same, ignoring case.
// call it like
//    if(streq_nocase(one, two))
int streq_nocase(const char* a, const char* b)
{
	for(; *a && *b; a++, b++)
		if(tolower(*a) != tolower(*b))
			return 0;

	return *a == 0 && *b == 0;
}

float input_conversion(int weight, const char* planet)
{
	float answer;

	if(streq_nocase(planet, "mercury")) {
		answer = (weight * 0.38);
		return answer; // it's mercury.
	}
	else if(streq_nocase(planet, "venus")) {
		answer = (weight * 0.91);
		return answer; // it's venus.
	}
	else if(streq_nocase(planet, "mars")) {
		answer = (weight * 0.38);
		return answer; // it's mars.
	}
	else if(streq_nocase(planet, "jupiter")) {
		answer = (weight * 2.54);
		return answer; // it's jupiter.
	}
	else if(streq_nocase(planet, "saturn")) {
		answer = (weight * 1.08);
		return answer; // it's saturn.
	}
	else if(streq_nocase(planet, "uranus")) {
		answer = (weight * 0.91);
		return answer; // it's uranus.
	}
	else if(streq_nocase(planet, "neptune")) {
		answer = (weight * 1.19);
		return answer; // it's neptune. 
	}
	else {
		return -1;
	}
}

int main()
{
	char prompt[20];
	printf("How much do you weigh?\n");
	fgets(prompt, sizeof(prompt), stdin); // ... use fgets to read a line into 'input'
	int oldWeight;
	sscanf(prompt, "%d", &oldWeight); // don't forget the '&'

	while(1) 
	{
		char input[50];
		read_line("What planet do you want to go to? (type 'exit' to exit): ", input, sizeof(input));
		if(streq_nocase(input, "exit"))
		{
			break;
		}
		else if(streq_nocase(input, "earth"))
		{
			printf("Already there.\n");
		}
		else 
		{
			float newWeight = input_conversion(oldWeight, input);
			printf("You'd weigh %.6f there.\n", newWeight);
			printf(" \n");
		}
	}
	return 0;
}