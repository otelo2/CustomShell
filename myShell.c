#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 80 /*Max lenght command*/

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; //command line arguments
    char history[10][38];
    char temp[MAX_LINE / 2 + 1];
    char *oldArgs[MAX_LINE / 2 + 1];
    int should_run, firstRun = 1; //flag to determine when to end program
    pid_t pidC1;
    int numOfArgs, numOfOldArgs, numPos, i = 0;
    int shouldWait = 0;
    int historyCounter = 0;

    while (should_run)
    {
        i = 0;
        printf("\nosh>");
        fflush(stdout);
        //Ask for user input source: https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array
        fgets(temp, sizeof temp, stdin);

        //If what the user typed doesnt have the "history" substring, add it to the history string
        if (strstr(temp, "history") == NULL)
        {
            //puts temp into the position historyCounter if the history string array has a !!
            if (strstr(temp, "!!") != NULL)
            {
                if (historyCounter == 0)
                {
                    int a = 1; //do nothing
                }
                else
                {
                    strcpy(history[historyCounter], history[historyCounter - 1]);
                    strcat(history[historyCounter], "\0\n");
                    historyCounter++;
                }
            }
            //just so it doesnt include the ! n command in history
            else if (strstr(temp, "!") != NULL)
            {
                if (historyCounter == 0)
                {
                    int a = 1; //do nothing
                }
                else
                {
                    //Super ugly but nothing else worked
                    switch (temp[2])
                    {
                    case '1':
                        strcpy(history[historyCounter], history[historyCounter - 1]);
                        strcat(history[historyCounter - 1], "\0\n");
                        historyCounter++;
                        break;
                    case '2':
                        strcpy(history[historyCounter], history[historyCounter - 2]);
                        strcat(history[historyCounter - 2], "\0\n");
                        historyCounter++;
                        break;
                    case '3':
                        strcpy(history[historyCounter], history[historyCounter - 3]);
                        strcat(history[historyCounter - 3], "\0\n");
                        historyCounter++;
                        break;
                    case '4':
                        strcpy(history[historyCounter], history[historyCounter - 4]);
                        strcat(history[historyCounter - 4], "\0\n");
                        historyCounter++;
                        break;
                    case '5':
                        strcpy(history[historyCounter], history[historyCounter - 5]);
                        strcat(history[historyCounter - 5], "\0\n");
                        historyCounter++;
                        break;
                    case '6':
                        strcpy(history[historyCounter], history[historyCounter - 6]);
                        strcat(history[historyCounter - 6], "\0\n");
                        historyCounter++;
                        break;
                    case '7':
                        strcpy(history[historyCounter], history[historyCounter - 7]);
                        strcat(history[historyCounter - 7], "\0\n");
                        historyCounter++;
                        break;
                    case '8':
                        strcpy(history[historyCounter], history[historyCounter - 8]);
                        strcat(history[historyCounter - 8], "\0\n");
                        historyCounter++;
                        break;
                    case '9':
                        strcpy(history[historyCounter], history[historyCounter - 9]);
                        strcat(history[historyCounter - 9], "\0\n");
                        historyCounter++;
                        break;
                    default:
                        break;
                    }
                    
                }
            }
            else
            {
                strcpy(history[historyCounter], temp);
                historyCounter++;
            }
        }

        //Turn string into token
        args[i] = strtok(temp, " ");
        while (args[i] != NULL)
        {
            args[++i] = strtok(NULL, " ");
        }
        numOfArgs = i;
        args[numOfArgs - 1] = strtok(args[numOfArgs - 1], "\n");

        //check for exit command
        if (strcmp("exit", args[0]) == 0)
        {
            should_run = 0;
            exit(0);
        }

        //Check for & command
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

                for (k = 0; k < historyCounter; k++)
                {
                    printf("%4d\t%s", historyCounter - k, history[k]);
                }
                printf("\n--------\n");
            }
        }

        //Check for !! command (excecute most recent command)
        if (strcmp("!!", args[0]) == 0)
        {
            //most recent: historyCounter-1
            //Turn string into token
            if (historyCounter < 1)
            {
                printf("No commands in history\n");
            }
            else
            {

                int k = 0;
                oldArgs[k] = strtok(history[historyCounter - 1], " ");
                while (oldArgs[k] != NULL)
                {
                    oldArgs[++k] = strtok(NULL, " ");
                }
                numOfOldArgs = k;
                oldArgs[numOfOldArgs - 1] = strtok(oldArgs[numOfOldArgs - 1], "\n");

                memcpy(args, oldArgs, sizeof(oldArgs));
            }
        }

        //Check for ! n command (excecute n past command)
        if (strcmp("!", args[0]) == 0)
        {
            //Turn the number from char to int and check if in historyCounter range
            if ((atoi(args[1]) > historyCounter))
            {
                printf("No such command in history\n");
            }
            else
            {
                int k = 0;
                oldArgs[k] = strtok(history[historyCounter - atoi(args[1])], " ");
                while (oldArgs[k] != NULL)
                {
                    oldArgs[++k] = strtok(NULL, " ");
                }
                numOfOldArgs = k;
                oldArgs[numOfOldArgs - 1] = strtok(oldArgs[numOfOldArgs - 1], "\n");

                memcpy(args, oldArgs, sizeof(oldArgs));
                /* 
                strcpy(history[historyCounter], history[historyCounter - atoi(args[1])]);
                strcat(history[historyCounter - atoi(args[1])], "\0\n");
                historyCounter++; */
            }
        }

        //Standard child handling
        pidC1 = fork();
        if (pidC1 < 0)
        {
            fprintf(stderr, "Fork failed");
        }
        else if (pidC1 == 0)
        {
            //This line can't be modified according to the document.
            execvp(args[0], args);
            exit(0);
        }
        else if (shouldWait == 1)
        {
            printf("\nDone.");
            wait(NULL);
            shouldWait = 0;
        }
        else
        {
            wait(NULL);
        }
    }
    return 0;
}