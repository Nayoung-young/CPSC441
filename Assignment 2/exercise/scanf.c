#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int main(int argc , char *argv[]){
    char msg[1000]; 

    printf("메시지 입력: "); 
    scanf("%[^\n]s", msg);
}