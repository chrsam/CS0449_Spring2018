// Chris Mananghaya (cam314)
// CS0449: Lab 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*PREDICATE)(const void*);

// ------------------------------------------------------

// For each item in the input array, call the predicate function with a pointer to that element.
// If it returned "true", use "memcpy" to copy that item from the input array to the output array. 
// Keep a count of how many items "passed the test" (predicate returned true), then return that count. 
int filter(void* output, const void* input, int length, int item_size, PREDICATE pred)
{
	// FILL ME IN!
	int count = 0;
	char* fil = (char*) input;
	char* fil_out = (char*) output;
	for(int i = 0; i < length; i++)
	{
		if(pred(input))
		{
			memcpy(output, input, item_size);
			count++;
			fil = fil + sizeof(float);
			fil_out = fil_out + sizeof(float);
			input = (void*) fil;
			output = (void*) fil_out;
		}
		else 
		{
			fil = fil + sizeof(float);
			input = (void*) fil;
		}
	}
	return count;

}

// interprets its parameter as a pointer to a float, then returns a "true" nonzero value if it is less than 50 
int less_than_50(const void* p)
{
	// FILL ME IN!
	float f = *(const float*)p; 
	if((f - 50) < 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// ------------------------------------------------------
// you shouldn't have to change the stuff below here.
// you can for testing, but please put it back the way it was before you submit.

#define NUM_VALUES 10

float float_values[NUM_VALUES] =
{
	31.94, 61.50, 36.10,  1.00,  6.35,
	20.76, 69.30, 19.60, 79.74, 51.29,
};

int main()
{
	float filtered[NUM_VALUES];
	int filtered_len = filter(filtered, float_values, NUM_VALUES, sizeof(float), &less_than_50);

	printf("there are %d numbers less than 50:\n", filtered_len);

	for(int i = 0; i < filtered_len; i++)
		printf("\t%.2f\n", filtered[i]);

	return 0;
}