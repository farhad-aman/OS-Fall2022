#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

const int SAMPLE_SIZE = 500000;
const int NUMBER_OF_PROCESSES = 2;

int main() 
{
    srand(time(0));

    int *hist = mmap(NULL, 25*sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    bool isTheMainParent = true;
    //*
    int id = fork();
    isTheMainParent = (id != 0);
    //*/
    
    for(int i = 0; i < SAMPLE_SIZE / NUMBER_OF_PROCESSES; i++) 
    {
        int counter = 0; 
        for(int j = 0; j < 12; j++) 
        {
            int random = rand() % 100;
            if(random < 50) 
            {
                counter--;
            } 
            else 
            {
                counter++;
            }
        }
        *(hist + (counter + 12)) = *(hist + (counter + 12)) + 1;
    }

    if(isTheMainParent) 
    {
	    wait(NULL);
	    printf("number of parent clocks: %ld\n", clock());
        int sum = 0;
        for(int i = 0; i < 25; i += 2) 
        {
            printf("%d \n", *(hist + i));
            sum += *(hist + i);
        }
        printf("sum: %d\n", sum);
	    printf("number of all the clocks taken: %ld\n", clock());
    } 
    else 
    {
	    printf("number of clocks taken: %ld for %d\n", clock(), getpid());
        exit(0);
    }
    return 0;
}
