/* AP buffer model for CPSC 441 Assignment 3 */
/* -- written by Nayoung Kim (UCID: 30169028) */

#include <stdio.h>

/* Buffer size in packets */
#define MAX_BUFFER 100 

/* Wireless link data rate <- link transmission rate? */ 
#define SPEED 3000000.0

typedef struct {
    float queue[MAX_BUFFER];
    int front, rear; 
} Queue; 

void queueInit(Queue* pQueue) {
    pQueue-> front = pQueue->rear = 0; 
    return;
}

int isFullQueue (Queue* pQueue) {
    if (((pQueue->rear)+1) % MAX_BUFFER == (pQueue->front)) 
        return 1; 
    return 0; 
}

int isEmptyQueue(Queue* pQueue){
    if (pQueue->front == pQueue->rear) return 1; 
    else return 0; 
}

int enqueue(Queue* pQueue, float val){
    if (isFullQueue(pQueue)){
        return 0; 
    }
    pQueue->queue[pQueue->rear] = val; 
    //printf("-Enqueued in ith index: %i\n", pQueue->rear);
    pQueue->rear = (pQueue->rear + 1) % (MAX_BUFFER); // rear update?
    return 1; 
}

int dequeue(Queue* pQueue) {
    if (isEmptyQueue(pQueue)){
        return 0;
    }
    pQueue->front = (pQueue->front+1) % MAX_BUFFER;
    return 1; 
}

float queueFront(Queue* pQueue) {
    return (pQueue->queue[pQueue->front]);
}


int main() {
    
    float now, prev, elapsed; // 시간? 

    int pktsin, pktsout, pktslots; 
    int bytesin, bytesout, byteslost;

    int pktsinq, bytesinq; // 얘네 둘은 뭐지 
    int maxpkts, maxbytes; 

    int pktSize;
    Queue sizesQueue; // 각 pkt의 size(bytes 저장)
    float atime;
    Queue atimesQueue; // 각 pkt의 도착 시간 저장 

    float departure;
    float totdelay, avgdelay;

    int totpkts; // 전체 pkt 수? 
    float totKB; // 전체 KB 수? <- loop 중에 계속 + 
    int credit; 

    int i = 0; // loop count 

    // -- Queue Init 
    queueInit(&sizesQueue);
    queueInit(&atimesQueue);

    pktsin = 0; pktsout = 0; pktslots = 0;
    while (fscanf(stdin, "%f %i", &atime, &pktSize) != EOF)
	{   
        // -- test 
        if (i> 5) break;
        else printf("The %dth LOOP\n", i);

        printf("now pkt atime, size: %f, %d\n", atime, pktSize);

        // dequeue (transmit)
        while(1) {
            //printf("This is DEQUEUE\n");
            if (isEmptyQueue(&atimesQueue)||isEmptyQueue(&sizesQueue)) {
                //printf("Queue is Empty\n");
                break;
            }

            if ((queueFront(&atimesQueue) + (queueFront(&sizesQueue)/SPEED)) <= atime) {
                printf("prev pkt atime: %f, size: %f\n", queueFront(&atimesQueue), queueFront(&sizesQueue));
                printf("prev pkt transmission time: %f\n", queueFront(&atimesQueue) + (queueFront(&sizesQueue)/SPEED));
                dequeue(&sizesQueue);
                dequeue(&atimesQueue);
                pktsout ++; 
                printf("DEQUEUE completed\n");
            } else { 
                break;
            }
            // delay 시간 업데이트 
        }

        // enqueue 
        if (!isFullQueue(&sizesQueue)){
            enqueue(&sizesQueue, pktSize);
            enqueue(&atimesQueue, atime);
            printf("ENQUEUE completed:: front - sizes: %i, times: %i\n", sizesQueue.front, atimesQueue.front); 
            printf("ENQUEUE completed:: rear - sizes: %i, times: %i\n", sizesQueue.rear, atimesQueue.rear); 
            pktsin++;
        } else {
            // if buffer is full 
        }

        i ++; 
        printf("pkts in, out, lots: %i, %i, %i\n\n", pktsin, pktsout, pktslots);
        //printf("%f\t%i\n", atime, size);
	}

    totpkts = i; 


	return 0;

    // while (fscanf(stdin, "%i\t%i", atimes[i++], sizes[i++]) != EOF)
	// {
	// 	printf("\n==============================\n");
	// 	printf("%s vs. %s\n", str1, str2);
	// 	printf("==============================\n");

	// 	distance = min_editdistance(str1, str2);

	// 	printf("\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	// }
}