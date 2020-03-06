#include <stdio.h>
#include <unistd.h>
#define MAX_LINE 80 /*Max lenght command*/

int main(void)
{
    char *args[MAX_LINE/2 +1]; //command line arguments
    int should_run = 1; //flag to determine when to end program

    while (should_run)
    {
        printf("jose>");
        fflush(stdout);
        //1. Fork child process with fork()
        //2. Child will invoce execvp()
        //3. If command included &, parent will wait();

    }
    return 0;
}