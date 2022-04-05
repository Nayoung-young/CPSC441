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
#define M 0 // # of bozons on the planet 
#define S 50.0 // Mean sleep duration 자는 시간 평균? 
#define Y 60.0 // Mean yodel duration 

// Simulation parameters 
#define WARM_UP 0.0
#define END_TIME 1000.0

// Bozon states 
#define SLEEPING 0
#define YODELING 1

// Yodel duration model 
#define EXPONENTIAL 1 
// #define CONSTANT 1 

// Debugging flags 
//#define DEBUG 1 

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

double mu; 
double Exponential(mu)
{
    double randnum, ans; 
    
    randnum = Uniform01();
    ans = -(mu)*log(randnum);
    return ans;
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
    nexteventindex = 0;
    for(i=0;i<M;i++){
        status[i] = SLEEPING;
        duration[i] = Exponential(S);

        if (duration[nexteventindex]>duration[i]) {
            nexteventindex = i;
        }
    }

    time_t starttime = time(NULL);
    double currdurtime = 0; 
    // start timer 
    while(starttime - time(NULL) == END_TIME) {
        currindex = nexteventindex; 
        currdurtime = duration[currindex];

        // 방금 있었던 공백에 대한 판단 
        if (activebozons > 1) {
            // screechy time 
            screechytime += (time(NULL) - mostrecentyodelstarttime);
        } else if (activebozons == 1) {
            // melodius time 
            melodiustime += (time(NULL) - mostrecentyodelstarttime);
            if (status[currindex] == YODELING) {
                // perfect time 
                perfectyodeltime += (time(NULL) - mostrecentyodelstarttime);
            }
        } else if (activebozons == 0) {
            // silent time 
            silenttime += (time(NULL) - mostrecentyodelstarttime);
        } else {
            prinf("ERROR: ACTIVE BOZONS count error\n");
            break; 
        } 

        // already yodelling -> stop yodelling 
        if (status[currindex] == YODELING) {
            status[currindex] == SLEEPING;
            duration[currindex] = Exponential(S);
            mostrecentyodelendtime = time(NULL);
            activebozons -= 1; 
        }
        // start yodelling 
        else {
            status[currindex] = YODELING;
            duration[currindex] += Exponential(Y);
            yodeltries += 1; 
            mostrecentyodelertostart = currindex;   
            mostrecentyodelstarttime = time(NULL);
            activebozons += 1;
        }

        // modify nexteventindex 
        for (i = 0; i < M; i++) {
            if (duration[currindex]>duration[i]) {
                nexteventindex = i;
            } 
        }

        usleep((duration[nexteventindex]-currdurtime) * 10^6);

    }


    // Summary 
    printf("M = %d, S = %f, Y = %f\n
            Total time observing channel: %f\n
            \tIdle time on the channel: %f %f%%\n
            \tMelodius time on channel: %f %f%%\n
            \tScreech time on the channel: %f %f%%\n\n
            
            \tAttempted yodels: %i\n
            \tPerfect yodels: %i\n
            \tPerfect yodels/Attempted yodels: (%i%%)\n
            \tPerfect yodel time on the channel: %f %f%%\n",
            M, S, Y,
            END_TIME,
            silenttime, silenttime/END_TIME*100,
            melodiustime, melodiustime/END_TIME*100,
            screechytime, screechytime/END_TIME*100, 
            yodeltries,
            perfectyodels,
            perfectyodels/yodeltries*100,
            perfectyodeltime, perfectyodeltime/END_TIME*100);


}
