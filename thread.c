/*
NAME: Daniel Thiem
ASSIGNMENT: Assignment 4
DUE DATE: 11/29/2023
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Size of the buffer
#define SIZE 15
#define NUM_ITEMS 12

// Special marker used to indicate end of the producer data
#define END_MARKER -1

// Buffer, shared resource
int buffer[SIZE];
int count = 0;
// Index where the producer will put the next item
int prod_idx = 0;
// Index where the consumer will pick up the next item
int con_idx = 0;
int myCount = 0;

// Initialize the mutex
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variables
pthread_cond_t myCond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t myCond2 = PTHREAD_COND_INITIALIZER;




/*
increment the count by one
*/
int put_item(int value)
{
    buffer[prod_idx] = value;
    prod_idx = (prod_idx + 1) % SIZE;
    count++;
    printf("myCount: %d -> ",myCount);
    fflush(stdin);
    fflush(stdout);
    myCount++;
    printf("%d\n",myCount);
    fflush(stdin);
    fflush(stdout);    

    return value;
}

/*
 Function that the producer thread will run. Loop and increment the count. 
*/
void *producer(void *args)
{
pthread_t hw_thread = pthread_self();
int value = 0;
while (myCount < 10)
    {
    pthread_mutex_lock(&myMutex); // lock mutex before checking condition
    printf("%d: myMutex locked\n",hw_thread);
    fflush(stdin);
    fflush(stdout);   
    while (count == SIZE){ // wait for signal
        printf("%d: waiting on myCond2",hw_thread);
        fflush(stdin);
        fflush(stdout);        
        pthread_cond_wait(&myCond2, &myMutex);
    
    }
    if (myCount < 10){
    put_item(value); //increment count
    }
    pthread_mutex_unlock(&myMutex); // unlock mutex
    printf("%d: myMutex unlocked\n",hw_thread);
    fflush(stdin);
    fflush(stdout); 
    printf("%d: signaling myCond1\n",hw_thread);
    fflush(stdin);
    fflush(stdout);       
    pthread_cond_signal(&myCond1); //signal consumer
    }    
    return NULL;
}


/*
 Consumer loop; alternates counting with producer.
*/
void *consumer(void *args)
{
printf("CONSUMER THREAD CREATED\n");
fflush(stdin);
fflush(stdout);   
pthread_t hw_thread = pthread_self();
int value = 0;
while (myCount < 10)
    {
    pthread_mutex_lock(&myMutex); //lock mutex before checking condition
    printf("%d: myMutex locked\n",hw_thread);
    fflush(stdin);
    fflush(stdout);   
    while (count == SIZE){ // wait for signal
        printf("%d: waiting on myCond1\n",hw_thread);
        fflush(stdin);
        fflush(stdout);        
        pthread_cond_wait(&myCond1, &myMutex);
    
    }

    if (myCount < 10){
        put_item(value); // increment count
    }
    pthread_mutex_unlock(&myMutex); // Unlock the mutex
    printf("%d: myMutex unlocked\n",hw_thread);
    fflush(stdin);
    fflush(stdout); 
    printf("%d: signaling myCond2\n",hw_thread);
    fflush(stdin);
    fflush(stdout);       
    pthread_cond_signal(&myCond2); //signal producer
    }    
    return NULL;
}

int main()
{
    printf("PROGRAM START\n");
    fflush(stdin);
    fflush(stdout);
    srand(time(0));
    pthread_t p, c;
    // Create the producer thread
    pthread_create(&p, NULL, producer, NULL);
    // Now create the consumer thread
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    printf("PROGRAM END\n");
    fflush(stdin);
    fflush(stdout);
    return 0;
}