#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define MAX_LINE 80 /*Max lenght command*/

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; //command line arguments
    int should_run = 1;           //flag to determine when to end program
    pid_t pidC1;
    char temp[MAX_LINE / 2 + 1];
    int numOfArgs,i = 0;

    while (should_run)
    {
        printf("jose>");
        fflush(stdout);
        //Ask for user input source: https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array
        fgets(temp, sizeof temp, stdin);
        args[i] = strtok(temp," ");
        while (args[i]!=NULL)
        {
            args[++i] = strtok(NULL," ");
        }
        numOfArgs=i;
        //Code to see what arguments we have (debugging)
        /*
        printf("%d\n",numOfArgs);
        for (i = 0; i < 6; i++)
        {
            printf(args[i]);
            printf("\n");
        } */
        
        //1. Fork child process with fork()
        pidC1 = fork();

        if (pidC1 < 0)
        {
            fprintf(stderr, "Fork failed");
        }
        else if (pidC1 == 0)
        {
            //printf("Child ID: %d\n", getpid());
            execvp("ps", "-ael");
        }
        else
        {
            wait(NULL);
            printf("Parent ID: %d\n", getppid());
        }
        //2. Child will invoce execvp()
        //3. If command included &, parent will wait();
    }
    return 0;
}