#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

int main(void)
{
	char *input = NULL;
	size_t input_size = 0;
	pid_t pid = fork();
	char **args = NULL;
	int status;
	ssize_t read_bytes = getline(&input, &input_size, stdin);

	while (1)
	{
		printf("(:");
		if (read_bytes == -1)
		{
			input[strcspn(input, "\n")] = '\0';
			args = malloc(2 * sizeof(char *));
			if (args == NULL)
			{
			perror("malloc");
			exit(EXIT_FAILURE);
			}
		}
		input[strcspn(input, "\n")] = '\0';
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (execvp(input, args) == -1)
			{
			perror("execvp");
			exit(EXIT_FAILURE);
			}
			args[0] = input;
			args[1] = NULL;
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
			{
			printf("Child process exited with status %d\n", WEXITSTATUS(status));
			}
		}
		printf("You typed: %s\n", input);
		}
	free(input);
	free(args);
	return (0);
}
