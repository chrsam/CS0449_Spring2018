// Chris Mananghaya (cam314)
// CS 0449: Project 4

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int get_num_args(char*);
char* delims = " ()<>|&;\t\n";

int get_num_args(char* input) // collects number of arguments 
{
  char arguments[300];
  char* current;
  int num_args = 0;

  strcpy(arguments, input);

  current = strtok(arguments, delims);
  while(current != NULL)
  {
    num_args++;
    current = strtok(NULL, delims);
  }
  return num_args;
}

void change_dir(char* new_direc) // handles directory change error cases
{
	char* dir_path = new_direc;

	if(dir_path == NULL)
	{
		printf("\t! Please specify a directory !\n");
	}
	else
	{
		int change_test = chdir(dir_path);
		if(change_test == -1)
			printf("\t! Error: Invalid directory !\n");
	}
}

int main() 
{
  char** arg_array;
  char* arg;
  char command_buffer[300];
  char args_buffer[300];
  int childpid = 0;
  int args_number = 0;
  int state;
  int count = 0;
  int value;

  signal(SIGINT, SIG_IGN);

  while(1) 
  {
    printf("myshell> ");
    fgets(command_buffer, 300, stdin);

    args_number = get_num_args(command_buffer);
    arg_array = malloc(sizeof(char*) * (args_number + 1));

    strcpy(args_buffer, command_buffer);

    arg = strtok(args_buffer, delims);
    count = 0;
    while(arg != NULL)
    {
      arg_array[count] = (char*) malloc(sizeof(char) * strlen(arg));
      strcpy(arg_array[count], arg);

      arg = strtok(NULL, delims);
      count++;
    }
    arg_array[args_number] = NULL;

    if(arg_array[0] == NULL)
      continue;

	if(!strcmp(arg_array[0], "exit") && (arg_array[1] != NULL)) // handles exit w/ additional arguments
	{
		arg = strtok(NULL, " ");
		sscanf(arg_array[1], "%d", &value);
		return value;
	}

	else if(!strcmp(arg_array[0], "exit")) // handles exit w/ no additional arguments 
	{
    	exit(0);
    }

    if(strcmp(arg_array[0], "cd") == 0)
    {
    	change_dir(arg_array[1]);
    }
    else 
    {
      if(fork() != 0) // handles rest of built-in commands (based on lab 7)
      {
      	signal(SIGINT, SIG_IGN);
        childpid = waitpid(-1, &state, 0);

        if(childpid == -1)
		{
			perror("Error running program");
		}
		else if(state == 0) {
			//printf("Program exited successfully!\n");
		}
		else if(WEXITSTATUS(state))
		{
			printf("\nExited successfully with exit code %d\n", WEXITSTATUS(state));
		}
		else if(signal(SIGINT, SIG_ERR))
		{
			printf("\n");
		}
		else
		{
			printf("Terminated some other way!\n");
		}
      }
      else 
      {
        if(strstr(command_buffer, ">>") != NULL)
        {
          freopen(arg_array[args_number-1], "a", stdout);

          free(arg_array[args_number-1]);
          arg_array[args_number-1] = NULL;
        }

        else if(strstr(command_buffer, ">") != NULL) // checks for output redirection
        {
          freopen(arg_array[args_number-1], "w", stdout);

          free(arg_array[args_number-1]);
          arg_array[args_number-1] = NULL;
        }
        else if(strstr(command_buffer, "<") != NULL) // checks for input redirection 
        {
          freopen(arg_array[args_number-1], "r", stdin);

          free(arg_array[args_number-1]);
          arg_array[args_number-1] = NULL;
        }
        signal(SIGINT, SIG_DFL);
        execvp(arg_array[0], arg_array);
        exit(0);
      }
    }
  }
  return 0;
}