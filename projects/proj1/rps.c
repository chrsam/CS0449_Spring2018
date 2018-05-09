// Chris Mananghaya (cam314)
// CS0449: Project 1 (part 1)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

// Program Logic:
// 1.) Have the player enter their choice.
// 2.) Have the computer randomly pick its choice.
// 3.) If the two match, it's a tie, and that round doesn't count.
// 4.) Otherwise, give the winner a point. 
// 5.) When one player reaches three points, they win and the tournament is over. 
// 6.) Ask the user if they want to play again. If so, go back to the beginning. 

// prints a message, reads a line of input, and chops off the newline.
void read_line(const char* message, char* buffer, int length)
{
	printf(message);
	fgets(buffer, length, stdin);
	if(strlen(buffer) != 0)
		buffer[strlen(buffer) - 1] = 0;
}

// sees if two strings are the same, ignoring case.
int streq_nocase(const char* a, const char* b)
{
	for(; *a && *b; a++, b++)
		if(tolower(*a) != tolower(*b))
			return 0;

	return *a == 0 && *b == 0;
}

int random_range(int low_bound, int high_bound)
{
	int rand_num = rand() % (high_bound - low_bound + 1) + low_bound;
	return rand_num;
}

int main()
{
	char follow_up[20];
	int i;
	int user_choice;
	int computer_choice;
	int user_score;
	int computer_score;
	srand((unsigned int)time(NULL));

	while(1)
	{
		for(i = 1; i < 6; i++)
		{
			if(user_score == 3 || computer_score == 3)
				break;
			while(1) 
			{
				char question[50];
				printf("Round %d! ", i);
				read_line("What's your choice? ", question, sizeof(question));
				if(streq_nocase(question, "rock"))
				{
					user_choice = 1;
					computer_choice = random_range(1, 3);
					if(computer_choice == 2)
					{
						printf("The computer chooses paper. You lose this round. ");
						computer_score++;
					}
					else if(computer_choice == 3)
					{
						printf("The computer chooses scissors. You win this round. ");
						user_score++;
					}
					else
					{
						printf("The computer chooses rock. It's a tie. ");
					}
					printf("You: %d, Computer: %d\n", user_score, computer_score);
					break;
				}
				else if(streq_nocase(question, "paper"))
				{
					user_choice = 2;
					computer_choice = random_range(1, 3);
					if(computer_choice == 1)
					{
						printf("The computer chooses rock. You win this round. ");
						user_score++;
					}
					else if(computer_choice == 3)
					{
						printf("The computer chooses scissors. You lose this round. ");
						computer_score++;
					}
					else
					{
						printf("The computer chooses paper. It's a tie. ");
					}
					printf("You: %d, Computer: %d\n", user_score, computer_score);
					break;
				}
				else if(streq_nocase(question, "scissors"))
				{
					user_choice = 3;
					computer_choice = random_range(1, 3);
					if(computer_choice == 1)
					{
						printf("The computer chooses rock. You lose this round. ");
						computer_score++;
					}
					else if(computer_choice == 2)
					{
						printf("The computer chooses paper. You win this round. ");
						user_score++;
					}
					else
					{
						printf("The computer chooses scissors. It's a tie. ");
					}
					printf("You: %d, Computer: %d\n", user_score, computer_score);
					break;
				}
				else
				{
					printf("Invalid response. Please try again.\n");
				}
			}
		}
		if(user_score > computer_score)
		{
			read_line("You won the tournament! Play again? ", follow_up, sizeof(follow_up));
		}
		else if(user_score < computer_score)
		{
			read_line("Sorry. You lost the tournament. Play again? ", follow_up, sizeof(follow_up));
		}
		else
		{
			read_line("You two tied. Guess no one wins. Play again? ", follow_up, sizeof(follow_up));
		}
		if(streq_nocase(follow_up, "no"))
		{
			break;
		}
		else
		{
			user_score = 0;
			computer_score = 0;
		}
	}
	return 0;
}