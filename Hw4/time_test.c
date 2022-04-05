/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

// void delay(clock_t n){

//   time_t start = time(NULL);
//   while(time(NULL) - start < n);

// }

int main()
{
    struct timeval  tv;
	double starttime, curtime;

	// 시작하는 시간 받아오기
// 	gettimeofday(&tv, NULL);
// 	starttime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

    printf("현재 시간: %ld\n", time(NULL));
	// 시간 측정을 진행할 부분
	usleep(1000000); // 1.5초
	printf("현재 시간: %ld\n", time(NULL));
	

	// 끝나는 시간 받아오기
// 	gettimeofday(&tv, NULL);
// 	curtime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

	// 출력
	//printf("Execution time %f\n", (curtime - starttime) / 1000);
    

    // time_t curtime; 
    // curtime = time(NULL);
    // printf("start time: %ld\n", curtime);
    
    // delay(1000);
    // printf("next time: %ld\n", time(NULL));
        
    // while(1) {
    //     delay(1000);
    //     printf("next time: %lld\n", time(NULL));
    // }
    
    return 0;
}
