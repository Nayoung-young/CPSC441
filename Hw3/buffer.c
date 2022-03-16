/* AP buffer model for CPSC 441 Assignment 3 */
/* -- written by Nayoung Kim (UCID: 30169028) */

#include <stdio.h>

/* Buffer size in packets */
#define MAX_BUFFER 100 

/* Wireless link data rate <- link transmission rate? */ 
#define SPEED 3000000.0

int main() {
    
    float now, prev, elapsed; // 시간? 

    int pktsin, pktsout, pktslots; 
    int bytesin, bytesout, byteslost;

    int pktsinq, bytesinq; // 얘네 둘은 뭐지 
    int maxpkts, maxbytes; 

    int size, sizes[MAX_BUFFER]; // 각 pkt의 size(bytes 저장)
    float atime, atimes[MAX_BUFFER]; // 각 pkt의 도착 시간 저장 

    float departure;
    float totdelay, avgdelay;

    int totpkts; // 전체 pkt 수? 
    float totKB; // 전체 KB 수? <- loop 중에 계속 + 
    int credit; 

    int i = 0; // loop count 

    while (fscanf(stdin, "%f\t%i", &atime, &size) != EOF)
	{
        // buffer full 
        // buffer in 
        i ++; 
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