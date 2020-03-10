#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 80 /*Max lenght command*/

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; //command line arguments
    int should_run, firstRun = 1; //flag to determine when to end program
    pid_t pidC1;
    char temp[MAX_LINE / 2 + 1];
    int numOfArgs, i = 0;
    int shouldWait = 0;

    //pidC1 = fork();

    while (should_run)
    {
        i = 0;
        printf("jose>");
        fflush(stdout);
        //Ask for user input source: https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array
        fgets(temp, sizeof temp, stdin);
        args[i] = strtok(temp, " ");
        while (args[i] != NULL)
        {
            args[++i] = strtok(NULL, " ");
        }
        numOfArgs = i;
        args[numOfArgs - 1] = strtok(args[numOfArgs - 1], "\n");

        //Code to see what arguments we have (debugging)
        //printf("%d\n",numOfArgs);
        for (i = 0; i < numOfArgs; i++)
        {
            printf("%s", args[i]);
            //printf("\n");
        }

        //1. Fork child process with fork()
        //pidC1 = fork();

        if (firstRun = 1)
        {
            firstRun = 0;
            pidC1 = fork();
            firstRun = 0; //asures we only get one fork
        }

        //check for exit command
        if (strcmp("exit", args[0]) == 0)
        {
            should_run = 0;
            exit(0);
        }

        if (strcmp("&", args[numOfArgs - 1]) == 0)
        {
            shouldWait=1;
            printf("There is a &");
        }

        //Standard child handling
        if (pidC1 < 0)
        {
            fprintf(stderr, "Fork failed");
        }
        else if (pidC1 == 0)
        {
            //printf("Child ID: %d\n", getpid());

            //This line can't be modified according to the document.
            execvp(args[0], args);
            exit(0);
        }
        else if (shouldWait == 1)
        {
            printf("Got here because of a &\n");
            wait(NULL);
        }
        else
        {
            wait(NULL);
        }

        //2. Child will invoce execvp()
        //3. If command included &, parent will wait();
    }
    return 0;
}