/* 
Name: SSTF Disk Scheduling Simulation
Author: Taylor McLean

Description:
This program simulates the shortest seek time first (SSTF) algorithm for disk
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
int removeSSTF(struct queue *requestQueue, int givenTrack);
void addSSTF(struct queue *requestQueue, int track);

/* MAIN FUNCTION */
int main()
{
    printf("SSTF DISK SCHEDULING\n");

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

        struct queue requestQueue;
        requestQueue.head = NULL;
        requestQueue.tail = NULL;

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
                addSSTF(&requestQueue, track);
            }

            // Serve next request
            int trackServed = removeSSTF(&requestQueue, currentTrack);

            // Calculate disk head movement and increment counters
            int diff = abs(currentTrack - trackServed);
            totalHeadMove += diff;
            totalRequests++;
            currentTrack = trackServed;
            numFileRequests++;
        }

        // Process remaining track requests on the queue
        while (requestQueue.tail != NULL)
        {
            // Serve next request
            int trackServed = removeSSTF(&requestQueue, currentTrack);

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

/* SSTF FUNCTIONS */

/* 
Algorithm: Start at the tail of the queue. Keep track of the track number of 
the current node as well as the previous. Compare the given track number to the
track number of the current node, and the track number of the previous node. If
the difference between given track number and current node track number is lower,
continue traversing. Otherwise, we know that the track number of the previous
node is lowest, so return that number and remove the node from the queue. This
algorithm works because the queue will be sorted based on increasing track number.
 */
int removeSSTF(struct queue *requestQueue, int givenTrack)
{
    struct node *currentNode = requestQueue->tail->next;
    struct node *previousNode;
    previousNode = requestQueue->tail;

    while (currentNode != NULL)
    {
        int currentTrack = currentNode->trackNum;
        int previousTrack = previousNode->trackNum;

        int difCurrent = abs(givenTrack - currentTrack);
        int difPrevious = abs(givenTrack - previousTrack);

        // if difPrevious is less than difCurrent, then we know that previousTrack
        // has the shortest seek time from the given track, so we are done.
        if (difPrevious <= difCurrent)
        {
            if (previousNode->previous != NULL)
            {
                previousNode->previous->next = previousNode->next;
            }
            previousNode->next->previous = previousNode->previous;

            // Check if node being removed is queue tail
            if (previousNode == requestQueue->tail)
            {
                requestQueue->tail = currentNode;
            }

            return previousTrack;
        }
        // Otherwise, check the next track request in queue
        else
        {
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
    }

    // If currentNode is set to a null pointer, then we know that the head of the
    // queue needs to be removed

    // If the queue is more than one request long
    if (previousNode->previous != NULL)
    {
        previousNode->previous->next = NULL;
        requestQueue->head = previousNode->previous;
    }
    // Otherwise we are removing the last request from the queue
    else
    {
        requestQueue->head = NULL;
        requestQueue->tail = NULL;
    }
    int track = previousNode->trackNum;
    return track;
}

/* 
Algorithm: We first create a track request node to add to the queue. Then,
we determine where the request should be added to the queue, depending on 
the track number. Since we want the queue to be sorted by ascending track 
number, we iterate through the queue and compare track numbers until one
is less than, and insert at this location.
 */
void addSSTF(struct queue *requestQueue, int track)
{
    struct node *trackRequest = malloc(sizeof(struct node));
    trackRequest->trackNum = track;
    trackRequest->next = NULL;
    trackRequest->previous = NULL;

    // If the queue is empty, then set the new request to both head and tail
    if (requestQueue->tail == NULL)
    {
        requestQueue->tail = trackRequest;
        requestQueue->head = trackRequest;
        return;
    }

    struct node *currentNode = requestQueue->tail;
    while (currentNode != NULL)
    {
        int currentTrack = currentNode->trackNum;
        if (track <= currentTrack)
        {
            // Insert behind the current node.
            trackRequest->next = currentNode;
            if (currentNode->previous != NULL)
            {
                trackRequest->previous = currentNode->previous;
                currentNode->previous->next = trackRequest;
            }
            currentNode->previous = trackRequest;
            // We then want to know if the current node is tail
            if (currentNode == requestQueue->tail)
            {
                requestQueue->tail = trackRequest;
            }

            return;
        }
        else
        {
            // Otherwise, check the next node in the queue
            currentNode = currentNode->next;
        }
    }

    // If currentNode is set to null, then we know to insert at
    // the head of the queue
    requestQueue->head->next = trackRequest;
    requestQueue->head = trackRequest;
    return;
}