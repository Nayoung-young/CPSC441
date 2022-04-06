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
#define M 2 // # of bozons on the planet 
#define S 50.0 // Mean sleep duration 자는 시간 평균? 
#define Y 60.0 // Mean yodel duration 

// Simulation parameters 
#define WARM_UP 0.0
#define END_TIME 30.0 // sec

// Bozon states 
#define SLEEPING 0
#define YODELING 1

// Yodel duration model 
#define EXPONENTIAL 1 
// #define CONSTANT 1 

// Debugging flags 
#define DEBUG 1
//#define DEBUG 0  

// Parameters for random number generation 
#define MAX_INT 2147483647 // Max 양수 2^31 -1 

// Generate a random floating point # uniformly distributed in [0,1]
double Uniform01()
{
    double randnum; 
    // get a random positive integer from random()
    randnum = (double) 1.0 * random();
    // divide by max int to get something in the range 0.0 to 1.0 
    randnum = randnum / (1.0 * MAX_INT);
    return (randnum);
}

// Generate a random floating point # from an exponential 
// distribution with mean mu. 
 
double Exponential(double mu)
{
    double randnum, ans; 
    
    randnum = Uniform01();
    ans = -(mu)*log(randnum);
    if (DEBUG) {printf("\t !! new dur %f generated\n", ans);}
    return ans;
}

void Checkperformance(double timep, int status, int bozons, double* screechy, double* mel, double* per, int* pernum, double* sil, double sttime) {
    if (bozons > 1) {
        // screechy time 
        *screechy += timep;
        if (DEBUG) { printf("\t\tCURRENT SCREECH: %f\n", *screechy);}
    } else if (bozons == 1) {
        // melodius time 
        *mel += timep;
        if (DEBUG) { printf("\t\tCURRENT MEL: %f\n", *mel);}
        if (status == YODELING) {
            // perfect time 
            *per += timep;
            *pernum += 1; 
            if (DEBUG) { printf("\t\tCURRENT PERFECT: %f\n", *per);}
        }
    } else if (bozons == 0) {
            // silent time 
        *sil += timep;
        if (DEBUG) { printf("\t\tCURRENT SILENT: %f\n", *sil);}
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
        duration[i] = Exponential(S);
        if (DEBUG) { printf("status[%d]: duration::%f\n", i, duration[i]); } 

        if (duration[nexteventindex]>duration[i]) {
            nexteventindex = i;
        }
    }
    if (DEBUG) { printf("\n"); }


    time_t starttime = mostrecentyodelstarttime= time(NULL);
    double currdurtime = 0; 
    double timepassed = duration[nexteventindex]; 

    // start timer 
    if (DEBUG) { printf("-- START LOOP: starttime: %ld\n", starttime); }
    usleep(timepassed * 1000000);


    while(1) {
        if (DEBUG) { printf("\tcurrent time: %ld\n", time(NULL)); }

        currindex = nexteventindex; 
        currdurtime = duration[currindex];

        Checkperformance(timepassed, status[currindex], activebozons, &screechytime, &melodiustime, &perfectyodeltime, &perfectyodels, &silenttime, mostrecentyodelstarttime);
        
        // already yodelling -> stop yodelling 
        if (status[currindex] == YODELING) {
            if (DEBUG) { printf("\tNow %dth bozon is sleeping\n", currindex);}
            status[currindex] == SLEEPING;
            duration[currindex] += Exponential(S);
            mostrecentyodelendtime = time(NULL);
            activebozons -= 1; 
        }
        // start yodelling 
        else {
            if (DEBUG) { printf("\tNow %dth bozon is yodelling\n", currindex);}
            status[currindex] = YODELING;
            duration[currindex] += Exponential(Y);
            yodeltries += 1; 
            mostrecentyodelertostart = currindex;   
            mostrecentyodelstarttime = time(NULL);
            activebozons += 1;
        }
        if (DEBUG) { printf("\t%d bozons in colony are active\n", activebozons);}

        // modify nexteventindex 
        for (i = 0; i < M; i++) {
            if (duration[currindex]>duration[i]) {
                nexteventindex = i;
            } 
        }

        if (duration[nexteventindex] > END_TIME) {
            Checkperformance(timepassed, status[nexteventindex], activebozons, &screechytime, &melodiustime, &perfectyodeltime, &perfectyodels, &silenttime, mostrecentyodelstarttime);
            break; 
        }

        if (DEBUG) { printf("\t-- nexteventindex: %d, duration: %f\n\n", nexteventindex, duration[nexteventindex]); }
        timepassed = (duration[nexteventindex]-currdurtime);
        usleep( timepassed * 1000000);
        
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
        printf("\tPerfect yodels/Attempted yodels: (%d%%)\n",
            perfectyodels/yodeltries*100);
    }
    
    printf("\tPerfect yodel time on the channel: %f %f%%\n",
            perfectyodeltime, perfectyodeltime/END_TIME*100);
    
    return 0; 
}
