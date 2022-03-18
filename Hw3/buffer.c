/* AP buffer model for CPSC 441 Assignment 3 */
/* -- written by Nayoung Kim (UCID: 30169028) */

#include <stdio.h>

/* Buffer size in packets */
#define MAX_BUFFER 100
// #define MAX_BUFFER 2

/* Wireless link data rate <- link transmission rate? bits/sec */ 
#define SPEED 8000000.0

#define QSIZE (MAX_BUFFER+1)
typedef struct {
    float queue[QSIZE];
    int front, rear; 
} Queue; 

void queueInit(Queue* pQueue) {
    pQueue-> front = 0;
    pQueue->rear = 0; 
    return;
}

int isFullQueue (Queue* pQueue) {
    if (((pQueue->rear)+1) % QSIZE == (pQueue->front)) {
        // printf("The queue is full now! - front: %i, rear: %i\n", 
        //     pQueue->front, pQueue->rear);
        return 1;
    } 
    // printf("The queue is NOT full - front: %i, rear: %i\n", 
    //         pQueue->front, pQueue->rear);
    return 0; 
}

int isEmptyQueue(Queue* pQueue){
    if (pQueue->front == pQueue->rear) return 1; 
    else return 0; 
}

void enqueue(Queue* pQueue, float val){
    if (isFullQueue(pQueue)){
        return; 
    }
    //printf("-Enqueued in ith index: %i\n", pQueue->rear);
    pQueue->rear = (pQueue->rear + 1) % (QSIZE); // rear update?
    pQueue->queue[pQueue->rear] = val; 
    return; 
}

void dequeue(Queue* pQueue) {
    if (isEmptyQueue(pQueue)) return;
    
    //printf("**before dequeue front: %i\n ", pQueue-> front);
    //printf("QSIZE: %i\n", QSIZE);
    pQueue->front = (((pQueue->front) + 1) % QSIZE);
    
    //printf("**after dequeue front: %i\n ", pQueue-> front);
    return;

}

float queueFront(Queue* pQueue) {
    int qFront = pQueue->front; 
    if (qFront == QSIZE-1) return pQueue->queue[0];
    else return (pQueue->queue[pQueue->front+1]);
}


int main() {
    
    //float now, prev, elapsed; // 시간? 

    int pktsin, pktsout, pktslost; 
    int bytesin, bytesout, byteslost;

    int pktsinq, bytesinq;
    int maxpkts, maxbytes; 

    int pktSize;
    Queue sizesQueue; // 각 pkt의 size(bytes 저장)
    float atime;
    Queue atimesQueue; // 각 pkt의 도착 시간 저장 

    //float departure; // ??
    float totdelay, avgdelay;

    int totpkts, totbytes; // 전체 pkt 수? 
    //int credit; // ??

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
    totbytes = 0;

    int fullcount = 0;
    while (fscanf(stdin, "%f %i", &atime, &pktSize) != EOF)
	{   
        // -- test 
        // if (i> 7) break;
        // else printf("The %dth LOOP\n", i);
        // printf("The %dth LOOP\n", i);

        //printf("now pkt atime, size: %f, %d\n", atime, pktSize);
        totbytes += pktSize ;

        // dequeue (transmit)
        while(1) {
            //printf("This is DEQUEUE\n");
            if (isEmptyQueue(&atimesQueue)) {
                //printf("Queue is Empty\n");
                break;
            }

            if ((queueFront(&atimesQueue) + (queueFront(&sizesQueue)*8/SPEED)) <= atime) {
                // printf("prev pkt atime: %f, size: %fbytes\n", queueFront(&atimesQueue), queueFront(&sizesQueue));
                // printf("prev pkt transmission time: %f\n", queueFront(&atimesQueue) + (queueFront(&sizesQueue)/SPEED)*8);
                
                pktsout ++; bytesout += queueFront(&sizesQueue);
                pktsinq --; bytesinq -= queueFront(&sizesQueue);

                totdelay += (queueFront(&sizesQueue)/SPEED)*8;

                dequeue(&sizesQueue);
                dequeue(&atimesQueue);
                // printf("After DEQUEUE - front: %i, rear: %i\n", sizesQueue.front, sizesQueue.rear);

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

            // printf("ENQUEUE completed:: front: %i, rear: %i\n", sizesQueue.front, sizesQueue.rear); 
            
            pktsin++; bytesin += pktSize;
            pktsinq++; bytesinq += pktSize; 

            // printf("pktsinq: %i, bytesinq = %i\n", pktsinq, bytesinq);
            if (maxpkts < pktsinq) maxpkts = pktsinq; 
            if (maxbytes < bytesinq) maxbytes = bytesinq; 

        } else {
            fullcount ++;
            // if buffer is full 
            pktslost++; byteslost += pktSize;
        }

        i++; 
        // printf("pkts in, out, lost: %i, %i, %i\n\n", pktsin, pktsout, pktslost);
        //printf("%f\t%i\n", atime, size);
	}

    // queue에 남아있는 pkt transmit
    while(!isEmptyQueue(&sizesQueue)) {

        // printf("DEQUEUE for left pkts in q\n");
        // printf("pkt atime: %f, size: %fbytes\n", queueFront(&atimesQueue), queueFront(&sizesQueue));
        // printf("pkt transmission time: %f\n", queueFront(&atimesQueue) + (queueFront(&sizesQueue)/SPEED)*8);

        pktsout ++; bytesout += queueFront(&sizesQueue);
        totdelay += (queueFront(&sizesQueue)/SPEED)*8;
        dequeue(&sizesQueue);
        dequeue(&atimesQueue);
    }

    // Summary 
    printf("Wireless Link Speed: %d bps\n\n", (int)SPEED);
    printf("AP Buffer Size: %i pkts\n", MAX_BUFFER);

    // 1. # of incoming packets 
    totpkts = i; 
    printf("Incoming(Total) traffic: %i pkts\t%i bytes\n", totpkts, totbytes);

    // 2. # of delivered packets
    printf("Outgoing        traffic: %i pkts\t%i bytes\n", pktsout, bytesout);
    // !!! 근데 lost 되었으면 incoming traffic != outgoing traffic 이어야 함 

    // 3. # of lost packets 
    //printf("Fullcount: %i\n", fullcount);
    printf("Discarded        traffic: %i pkts\t%i bytes\n", pktslost, byteslost);

    // 4. packet loss percentage <- packetlosts /totpackets
    printf("Lost             traffic: %f%% pkts\n", ((double)pktslost / (double)totpkts * 100));

    printf("Peak occupancy: %d pkts\t%d bytes\n", maxpkts, maxbytes); 

    // 5. avg delay
    avgdelay = totdelay / totpkts;
    printf("Average queueing delay: %f sec\n", avgdelay);

    printf("Summary: %i %i %i %i %i %i %i %i %f %d %d %f\n", 
        (int)SPEED, MAX_BUFFER, totpkts, totbytes, pktsout, bytesout, pktslost, byteslost, 
        ((double)pktslost / (double)totpkts * 100), maxpkts, maxbytes, avgdelay);

	return 0;

}
