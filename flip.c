/* 
 * Operating Systems [2INCO] Practical Assignment
 * Threaded Application
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>          // for perror()
#include <pthread.h>

#include "uint128.h"
#include "flip.h"

// create a bitmask where bit at position n is set
#define BITMASK(n)          (((uint128_t) 1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v,n)     (((v) & BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v,n)        ((v) =  (v) |  BITMASK(n))

// clear bit n in v
#define BIT_CLEAR(v,n)      ((v) =  (v) & ~BITMASK(n))

// declare a mutex, and it is initialized as well
static pthread_mutex_t      mutex          = PTHREAD_MUTEX_INITIALIZER;

int 		    currentThreads = 0;      // number of current threads created
pthread_t   	threadId[NROF_THREADS];  // array of thread ID


// flips multiples of the received integer
static void * flip (void * arg) 
{
	int *   argi; 
	int     i;      

	argi = (int *) arg;  // proper casting before dereferencing
	i = *argi;           // get the integer value of the pointer   
	free (arg);          // we retrieved the integer value, so now the pointer can be deleted

	for (int j = 1; i * j <= NROF_PIECES; j++) 
	{
		pthread_mutex_lock (&mutex);

		// here we are in the critical section (protected by a mutex)
		if (BIT_IS_SET(buffer[(i * j)/128], (i * j) % 128))  // if the corresponding piece is black
		{
			BIT_CLEAR(buffer[(i * j)/128], (i * j) % 128);  // flip it to white
		}
		else 
		{
			BIT_SET(buffer[(i * j)/128], (i * j) % 128);  // else, flip it to black
		}

		pthread_mutex_unlock (&mutex);
	}

	return(0);
}

int main (void)
{
	// TODO: start threads to flip the pieces and output the results
	// (see thread_test() and thread_mutex_test() how to use threads and mutexes,
	//  see bit_test() how to manipulate bits in a large integer)


	// sets all bits(pieces) black in the buffers
	for (int p = 0; p <= NROF_PIECES; p++) 
	{
		BIT_SET(buffer[p/128], p % 128);
	}
    
	// flip every number's multiples
	for (int i = 2; i <= NROF_PIECES; i++) 
	{
		int *       parameter;  // parameter to be handed over to the thread
		parameter = malloc (sizeof (int));  // memory will be freed by the child-thread
		*parameter = i;  // integer of which multiples are to be flipped
		int			index = 0; // index of threadID array for which the main thread waits

		// When number of threads created reaches NROF_THREADS, wait for the oldest thread to stop
		if (currentThreads == NROF_THREADS) 
		{
			pthread_join (threadId[index], NULL);
			
			currentThreads--;  // decrement thread counter
			index++; // increment index of threadID array for which the main thread waits
			if (index == NROF_THREADS) 
			{
				index = 0; // index does not get larger than the largest index of threadID array
			}
		}
		
		pthread_create (&threadId[(i-2) % NROF_THREADS], NULL, flip, parameter);  // create new thread
		
		currentThreads++;  // increment thread counter
	}

	// wait till all the remaining threads are stopped
	for (int c = 0; c < currentThreads; c++) 
	{
		pthread_join (threadId[c], NULL);	
	}

	// print all number of black pieces in ascending order
	for (int p = 1; p <= NROF_PIECES; p++) 
	{
		if (BIT_IS_SET(buffer[p/128], p % 128)) 
		{
			printf("%d\n", p);
		}	
	}

    return (0);
}