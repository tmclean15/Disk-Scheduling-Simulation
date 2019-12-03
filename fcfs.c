/* 
Name: FCFS Disk Scheduling Simulation
Author: Taylor McLean

Description:
This program simulates the first come first served (FCFS) algorithm for disk
scheduling. It takes a number of file requests and a seed for the random number
generator as input, and takes the average head movement of 5 simulation runs 
with different seeds as output.
 */

#include <stdio.h>
#include <stdlib.h>

struct node
{
    int trackNum;
    struct node *previous;
    struct node *next;
};

struct queue
{
    struct node *head;
    struct node *tail;
};

/* FUNCTION DECLARATIONS */
int removeFCFS(struct queue *requestQueue);
void addFCFS(struct queue *requestQueue, int track);

/* MAIN FUNCTION */
int main()
{
    printf("FCFS DISK SCHEDULING\n");

    // Get user input for number of file requests
    int totalNumFileRequests;
    printf("Enter a number of file requests: ");
    scanf("%d", &totalNumFileRequests);

    int sumAverages;

    // We want to get an average of 5 iterations
    int i;
    for (i = 0; i < 5; i++)
    {
        // Get user input for random number generator
        int seed;
        printf("Enter a seed: ");
        scanf("%d", &seed);
        srand(seed);

        // Initialize queue
        struct queue requestQueue;
        requestQueue.head = NULL;
        requestQueue.tail = NULL;

        // Initialize counters and tracker variables
        int totalRequests = 0;
        int numFileRequests = 0;
        int totalHeadMove = 0;
        int currentTrack = 0;

        while (numFileRequests <= totalNumFileRequests)
        {
            int numTrackRequests = (rand() % 4) + 1; // Provide random number between 1 and 5
            // Generate track requests for this file request and add to queue
            int j;
            for (j = 0; j < numTrackRequests; j++)
            {
                int track = rand() % 800;
                addFCFS(&requestQueue, track);
            }

            // Serve next request
            int trackServed = removeFCFS(&requestQueue);

            // Calculate disk head movement and increment counters
            int diff = abs(currentTrack - trackServed);
            totalHeadMove += diff;
            totalRequests++;
            currentTrack = trackServed;
            numFileRequests++;
        }

        // Process remaining track requests on the queue
        while (requestQueue.head != NULL)
        {
            // Serve next request
            int trackServed = removeFCFS(&requestQueue);

            // Calculate disk head movement and increment counters
            int diff = abs(currentTrack - trackServed);
            totalHeadMove += diff;
            totalRequests++;
            currentTrack = trackServed;
        }

        // Calculate the average head move per request
        int average = totalHeadMove / totalRequests;
        // Add the average to the sum of averages
        sumAverages += average;
        printf("average: %d \n", average);
    }

    // Print result
    int finalAverage = sumAverages / 5;
    printf("The final average is %d \n", finalAverage);

    return 0;
}

/* FCFS FUNCTIONS */

/* 
Algorithm: The next request to be serviced will always be
at the head of the queue for FCFS, so the request node simply
needs to be removed and its track number returned.
 */
int removeFCFS(struct queue *requestQueue)
{
    struct node *headRequest = requestQueue->head;
    int track = headRequest->trackNum; // The track number that the disk head is moved to

    // If there is only one request left on queue
    if (headRequest->previous == NULL)
    {
        requestQueue->head = NULL;
        requestQueue->tail = NULL;
    }
    // Otherwise there is more than one request on the queue
    else
    {
        requestQueue->head = requestQueue->head->previous; // Remove the head of the queue
    }

    free(headRequest);
    return track;
}

/* 
Algorithm: For FCFS, adding a new requestputs it at the back 
of the queue, since removing requests occurs at the head of 
the queue. 
 */
void addFCFS(struct queue *requestQueue, int track)
{
    struct node *newRequest = malloc(sizeof(struct node)); // Create new request

    /* Populate request with track number and pointers */
    newRequest->trackNum = track;
    newRequest->previous = NULL;
    newRequest->next = NULL;

    // If the queue is empty
    if (requestQueue->tail == NULL)
    {
        requestQueue->tail = newRequest;
        requestQueue->head = newRequest;
    }
    // Otherwise the queue is not empty.
    else
    {
        newRequest->next = requestQueue->tail;
        requestQueue->tail->previous = newRequest;
        requestQueue->tail = newRequest;
    }

    return;
}