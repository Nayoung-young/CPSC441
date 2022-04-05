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
    int activeindex, nexteventindex, status[M];
    double duration[M];

    int yodeltries = 0, perfectyodels = 0;
    int mostrecentyodelertostart = 0;
    double mostrecentyodelstarttime = 0, mostrecentyodelendtime = 0; 

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
    // start timer 
    while(starttime - time(NULL) == END_TIME) {
        activeindex = nexteventindex;
        usleep(duration[activeindex] * 10^6);

        // already yodelling -> stop yodelling 
        if (status[activeindex] == YODELING) {
            status[activeindex] == SLEEPING;
            duration[activeindex] = Exponential(S);
            mostrecentyodelendtime = time(NULL);
        }
        // start yodelling 
        else {
            duration[activeindex] += Exponential(Y);
            status[activeindex] = YODELING;
            yodeltries += 1; 
            mostrecentyodelertostart = activeindex;
            mostrecentyodelstarttime = time(NULL);
        }
        
        // yodelling이 겹치면? ** 작성 필요 

        // modify nexteventindex 
        for (i = 0; i < M; i++) {
            if (duration[activeindex]>duration[i]) {
                nexteventindex = i;
            } 
        }


    }


    // Summary 
    printf("M = %d, S = %f, Y = %f\n
            Total time observing channel: %f\n
            \tIdle time on the channel: %f %f%%\n
            \tMelodius time on channel: %f %f%%\n
            \tScreech time on the channel: %f %f%%\n\n
            
            \tAttempted yodels: %i\n
            \tPerfect yodels: %i\n
            \tPerfect yodels/Attempted yodels: %i (%i%%)\n
            \tPerfect yodel time on the channel: %f %f%%\n",
            0, 0, 0,
            0,
            0, 0,
            0, 0,
            0, 0, 
            0,
            0,
            0, 0,
            0, 0);


}
