// 컴파일 후 
// !! -lm  <- math library 때문에? 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
//#include <windows.h>

// Constants 
#define M 3 // # of bozons on the planet 
#define S 12.0 // Mean sleep duration 자는 시간 평균? 
#define Y 6.0 // Mean yodel duration 

// Simulation parameters 
#define WARM_UP 0.0
#define END_TIME 30.0 // 

// Bozon states 
#define SLEEPING 0
#define YODELING 1

// Yodel duration model 
#define EXPONENTIAL 1 
// #define CONSTANT 1 

// Debugging flags 
#define DEBUG 1
//#define DEBUG 0  

/* Parameters for random number generation. */
#define MAX_INT 2147483647       /* Maximum positive integer 2^31 - 1 */

/* Generate a random floating point number uniformly distributed in [0,1] */
double Uniform01()
  {
    double randnum;
    /* get a random positive integer from random() */
    randnum = (double) 1.0 * random();
    /* divide by max int to get something in the range 0.0 to 1.0  */
    randnum = randnum / (1.0 * MAX_INT);
    return( randnum );
  }

/* Generate a random floating point number from an exponential    */
/* distribution with mean mu.                                     */
double Exponential(mu)
    double mu;
 {
    double randnum, ans;

    randnum = Uniform01();
    ans = -(mu) * log(randnum);
    return( ans );
  }

void Checkperformance(double pretime, double timep, int status, double dur, int bozons, double* screechy, double* mel, double* per, int* pernum, double* sil, double sttime) {
    double tmp; 
    if (bozons > 1) {
        // screechy time 
        tmp = *screechy;
        *screechy += timep;
        if (DEBUG) { printf("\t\tCURRENT SCREECH:%f +%f= %f\n", tmp ,timep,*screechy);}
    } else if (bozons == 1) {
        // melodius time 
        tmp = *mel;
        *mel += timep;
        if (DEBUG) { printf("\t\tCURRENT MEL:%f +%f= %f\n", tmp, timep, *mel);}
        if (status == YODELING && dur == (pretime+timep)) {
            // perfect time 
            tmp = *per;
            *per += timep;
            *pernum += 1; 
            if (DEBUG) { printf("\t\tCURRENT PERFECT:%f +%f= %f\n", tmp, timep, *per);}
        }
    } else if (bozons == 0) {
            // silent time 
        tmp = *sil;
        *sil += timep;
        if (DEBUG) { printf("\t\tCURRENT SILENT:%f +%f= %f\n", tmp, timep, *sil);}
    } else {
        printf("ERROR: ACTIVE BOZONS count error\n");
    }
    return; 
}

int main () 
{
    int i;
    int currindex, nexteventindex, status[M];
    int activebozons = 0;
    double duration[M];

    int yodeltries = 0, perfectyodels = 0;
    int mostrecentyodelertostart = 0;
    double mostrecentyodelstarttime = 0, mostrecentyodelendtime = 0; 

    // parameters for checking performance 
    double silenttime = 0, melodiustime = 0, screechytime = 0;
    double perfectyodeltime = 0;  

    // setup status[M], duration[M]
    if (DEBUG) { printf("--SET UP status[M], duration[M]\n");}
    nexteventindex = 0;
    for(i=0;i<M;i++){
        status[i] = SLEEPING;
        duration[i] = Exponential(S); // 왜 여기서 8.5, 7, 9가 안 나오지? 이거 TA한테 메일보내보자 
        if (DEBUG) { printf("status[%d]: duration::%f\n", i, duration[i]); } 

        if (duration[nexteventindex]>duration[i]) {
            nexteventindex = i;
        }
    }
    if (DEBUG) { printf("\n"); }


    // time_t starttime = mostrecentyodelstarttime= time(NULL);
    double currdurtime = 0, prevtime = 0; 
    double timepassed = duration[nexteventindex]; 

    // start timer 
    if (DEBUG) { printf("-- START LOOP: starttime: 0\n"); }
    // if (DEBUG) { printf("-- START LOOP: starttime: 0\n", starttime); }
    // usleep(timepassed * 1000000);

    int j = 0; 
    while(1) {
        // if (DEBUG) { printf("\tcurrent time: %ld\n", time(NULL)); }
        if (DEBUG) {printf("\t%dth LOOP\n\n", j++);}
        currindex = nexteventindex; 
        currdurtime = duration[currindex];

        if (DEBUG) { printf("\tcurrent time: %f\n", currdurtime); }

        Checkperformance(prevtime, timepassed, status[currindex], currdurtime, activebozons, &screechytime, &melodiustime, &perfectyodeltime, &perfectyodels, &silenttime, mostrecentyodelstarttime);
        
        // already yodelling -> stop yodelling 
        if (status[currindex] == YODELING) {
            if (DEBUG) { printf("\tNow %dth bozon is sleeping\n", currindex);}
            status[currindex] = SLEEPING;
            duration[currindex] += Exponential(S);
            // mostrecentyodelendtime = time(NULL);
            mostrecentyodelendtime = currdurtime;
            activebozons -= 1; 
        }
        // start yodelling 
        else {
            if (DEBUG) { printf("\tNow %dth bozon is yodelling\n", currindex);}
            status[currindex] = YODELING;
            duration[currindex] += Exponential(Y);
            yodeltries += 1; 
            mostrecentyodelertostart = currindex;   
            // mostrecentyodelstarttime = time(NULL);
            mostrecentyodelstarttime = currdurtime;
            activebozons += 1;
        }
        if (DEBUG) { printf("\t%d bozons in colony are active\n", activebozons);}

        // modify nexteventindex 
        for (i = 0; i < M; i++) {
            if (duration[currindex]>duration[i]) {
                nexteventindex = i;
            } 
        }

        if (DEBUG) { printf("\t-- nexteventindex: %d, duration: %f\n\n", nexteventindex, duration[nexteventindex]); }

        prevtime = currdurtime; 
        if (duration[nexteventindex] > END_TIME) {
            Checkperformance(prevtime, END_TIME-currdurtime, status[nexteventindex], duration[nexteventindex], activebozons, &screechytime, &melodiustime, &perfectyodeltime, &perfectyodels, &silenttime, mostrecentyodelstarttime);
            break; 
        }
        timepassed = (duration[nexteventindex]-currdurtime);
        
    }
    if (DEBUG) {printf("LOOP ENDED\n\n");}

    // Summary 
    printf("M = %d, S = %f, Y = %f\n", M, S, Y); 
    printf("Total time observing channel: %f\n", END_TIME);
    printf("\tIdle time on the channel: %f %f%%\n",
            silenttime, silenttime/END_TIME*100);
    printf("\tMelodius time on channel: %f %f%%\n",
            melodiustime, melodiustime/END_TIME*100);
    printf("\tScreech time on the channel: %f %f%%\n\n",
            screechytime, screechytime/END_TIME*100);
    printf("\tAttempted yodels: %d\n",
            yodeltries);
    printf("\tPerfect yodels: %d\n",
            perfectyodels);
    if (yodeltries == 0) {
        printf("\tPerfect yodels/Attempted yodels: (nan%%)\n");
    } else {
        printf("\tPerfect yodels/Attempted yodels: %f (%f%%)\n",
            (double)perfectyodels/(double)yodeltries, (double)perfectyodels/(double)yodeltries*100);
    }
    
    printf("\tPerfect yodel time on the channel: %f %f%%\n",
            perfectyodeltime, perfectyodeltime/END_TIME*100);
    
    return 0; 
}
