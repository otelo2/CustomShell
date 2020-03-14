#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 80 /*Max lenght command*/

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; //command line arguments
    char history[200];           //Sorry not sorry
    char temp[MAX_LINE / 2 + 1];
    char *oldArgs[MAX_LINE / 2 + 1];
    int should_run, firstRun = 1; //flag to determine when to end program
    pid_t pidC1;
    int numOfArgs, i = 0;
    int shouldWait = 0;
    int historyCounter = 0;

    //pidC1 = fork();

    while (should_run)
    {
        i = 0;
        printf("\nshell>");
        fflush(stdout);
        //Ask for user input source: https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array
        fgets(temp, sizeof temp, stdin);

        //If what the user typed doesnt have the "history" substring, add it to the history string
        if (strstr(temp, "history") == NULL)
        {
            strcat(history, temp);
            strcat(history, " \n");
            historyCounter++;
        }

        //Turn string into token
        args[i] = strtok(temp, " ");
        while (args[i] != NULL)
        {
            args[++i] = strtok(NULL, " ");
        }
        numOfArgs = i;
        args[numOfArgs - 1] = strtok(args[numOfArgs - 1], "\n");

        //Code to see what arguments we have (debugging)
        //printf("%d\n",numOfArgs);
        /* for (i = 0; i < numOfArgs; i++)
        {
            printf("%s", args[i]);
            printf("\n");
        } */

        //1. Fork child process with fork()
        //pidC1 = fork();

        //check for exit command
        if (strcmp("exit", args[0]) == 0)
        {
            should_run = 0;
            exit(0);
        }

        //Check for &
        if (strcmp("&", args[numOfArgs - 1]) == 0)
        {
            shouldWait = 1;
            args[numOfArgs - 1] = NULL;
        }

        //Check for history command
        if (strcmp("history", args[0]) == 0)
        {
            if (historyCounter == 0)
            {
                printf("Nothing in history\n");
            }
            else
            {
                int k = 0;
                oldArgs[k] = strtok(history, "\n");
                while (oldArgs[k] != NULL)
                {
                    oldArgs[++k] = strtok(NULL, "\n");
                }

                for (k = 0; k < historyCounter; k++)
                {
                    printf("%4d\t%s\n", historyCounter - k, oldArgs[k]);
                }
            }
        }

        //Chec for !! command
        if (strcmp("!!", args[0]) == 0)
        {
            printf("Check: %d", strcmp("!!", args[0]));
            printf("\n! wow");
        }

        //Standard child handling
        pidC1 = fork();
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
            printf("Waiting...");
            wait(NULL);
            shouldWait = 0;
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