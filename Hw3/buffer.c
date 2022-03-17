/* AP buffer model for CPSC 441 Assignment 3 */
/* -- written by Nayoung Kim (UCID: 30169028) */

#include <stdio.h>

/* Buffer size in packets */
#define MAX_BUFFER 100 

/* Wireless link data rate <- link transmission rate? bits/sec */ 
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

    int pktsin, pktsout, pktslost; 
    int bytesin, bytesout, byteslost;

    int pktsinq, bytesinq; // ??
    int maxpkts, maxbytes; 

    int pktSize;
    Queue sizesQueue; // 각 pkt의 size(bytes 저장)
    float atime;
    Queue atimesQueue; // 각 pkt의 도착 시간 저장 

    float departure; // ??
    float totdelay, avgdelay;

    int totpkts; // 전체 pkt 수? 
    float totKB; // 전체 KB 수? <- loop 중에 계속 + 
    int credit; // ??

    int i = 0; // loop count 

    // -- Queue Init 
    queueInit(&sizesQueue);
    queueInit(&atimesQueue);

    // -- pkts Arrive 
    pktsin = 0; pktsout = 0; pktslost = 0;
    bytesin = 0; bytesout = 0; byteslost = 0;
    totdelay = 0; avgdelay = 0; 
    maxpkts = 0; maxbytes = 0;
    pktsinq = 0; bytesinq = 0;
    
    while (fscanf(stdin, "%f %i", &atime, &pktSize) != EOF)
	{   
        // -- test 
        // if (i> 5) break;
        // else printf("The %dth LOOP\n", i);
        // printf("The %dth LOOP\n", i);

        // printf("now pkt atime, size: %f, %d\n", atime, pktSize);

        // dequeue (transmit)
        while(1) {
            //printf("This is DEQUEUE\n");
            if (isEmptyQueue(&atimesQueue)||isEmptyQueue(&sizesQueue)) {
                //printf("Queue is Empty\n");
                break;
            }

            if ((queueFront(&atimesQueue) + (queueFront(&sizesQueue)*8/SPEED)) <= atime) {
                // printf("prev pkt atime: %f, size: %fbytes\n", queueFront(&atimesQueue), queueFront(&sizesQueue));
                // printf("prev pkt transmission time: %f\n", queueFront(&atimesQueue) + (queueFront(&sizesQueue)/SPEED)*8);
                
                pktsout ++; bytesout += queueFront(&sizesQueue);
                pktsinq --; bytesinq -= queueFront(&sizesQueue);

                totdelay += (queueFront(&atimesQueue) + (queueFront(&sizesQueue)/SPEED)*8);

                dequeue(&sizesQueue);
                dequeue(&atimesQueue);


                // printf("DEQUEUE completed\n");
            } else { 
                break;
            }
            // delay 시간 업데이트 
        }

        // enqueue 
        if (!isFullQueue(&sizesQueue)){
            enqueue(&sizesQueue, pktSize);
            enqueue(&atimesQueue, atime);

            // printf("ENQUEUE completed:: front - sizes: %i, times: %i\n", sizesQueue.front, atimesQueue.front); 
            // printf("ENQUEUE completed:: rear - sizes: %i, times: %i\n", sizesQueue.rear, atimesQueue.rear); 
            
            pktsin++; bytesin += pktSize;
            pktsinq ++; bytesinq += pktSize; 

            if (maxpkts < pktsinq) maxpkts = pktsinq; 
            if (maxbytes < bytesinq) maxbytes = bytesinq; 

        } else {
            // if buffer is full 
            pktslost++; byteslost += pktSize;
        }

        i ++; 
        // printf("pkts in, out, lots: %i, %i, %i\n\n", pktsin, pktsout, pktslost);
        //printf("%f\t%i\n", atime, size);
	}

    // queue에 남아있는 pkt transmit
    while(!isEmptyQueue(&sizesQueue)) {
        pktsout ++; bytesout += queueFront(&sizesQueue);
        totdelay += queueFront(&atimesQueue) + (queueFront(&sizesQueue)/SPEED)*8;
        dequeue(&sizesQueue);
        dequeue(&atimesQueue);
    }


    // Summary 
    printf("AP Buffer Size: %i pkts\n", MAX_BUFFER);
    printf("Wireless Link Speed: %f bps\n\n", SPEED);

    // 1. # of incoming packets 
    totpkts = i; 
    printf("Incoming traffic: %i pkts\t%i bytes\n", pktsin, bytesin);

    // 2. # of delivered packets
    printf("Outgoing traffic: %i pkts\t%i bytes\n", pktsout, bytesout);

    // 3. # of lost packets 
    printf("Discarded traffic: %i pkts\t%i bytes\n", pktslost, byteslost);

    // 4. packet loss percentage <- packetlosts /totpackets
    printf("Lost traffic: %f%% pkts\n", ((double)pktslost / (double)totpkts * 100));

    printf("Peak occupancy: %d pkts\t%d bytes\n", maxbytes, maxpkts);

    // 5. avg delay
    avgdelay = totdelay / totpkts;
    printf("Average queueing delay: %f sec\n", avgdelay);

    printf("Summary: ~~ ??");

	return 0;

}