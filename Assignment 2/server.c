#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

#define SERVER_IP "127.0.0.1"
//#define SERVER_IP "136.159.5.27" 

#define PORT_TCP 9000
#define PORT_UDP 8888

#define VERSION "SIMPLE"
//#define VERSION "ADVANCED"

int main(int argc, char *argv[]){

    printf("=============================\n");
    printf("Hello, this is vowelizer Server\n");
    printf("=============================\n\n");

    int socket_TCP, socket_UDP, client_TCP; 
    struct sockaddr_in server_TCP, server_UDP;
        //client_sock_UDP, client_sock_TCP; 
    char client_vowel[5000], client_nonvowel[5000];

    // create TCP 
    socket_TCP = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_TCP == -1)
	{
		printf("Could not create socket\n");
	}

    server_TCP.sin_family = AF_INET;
	server_TCP.sin_addr.s_addr = INADDR_ANY;
	server_TCP.sin_port = htons(PORT_TCP);

    int bindStatus = 0;
    bindStatus = bind(socket_TCP, (struct sockaddr *)&server_TCP, sizeof(server_TCP));
	if( bindStatus == -1){
		//print the error message
		perror("TCP Binding failed!!");
		return 1;
	}

    listen(socket_TCP, 3);

    puts("TCP Socket is running now...");

    // connect Client - TCP
    client_TCP = accept(socket_TCP, NULL, NULL);
    if (client_TCP < 0 ){
		perror("client_TCP Connection failed\n");
		return 1;
	}
    printf("TCP Client Connection accepted...\n");

    // crate UDP 
    socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_UDP == -1)
	{
		printf("Could not create socket\n");
	}

    server_UDP.sin_family = AF_INET;
	server_UDP.sin_addr.s_addr = INADDR_ANY;
	server_UDP.sin_port = htons(PORT_UDP);

    bindStatus = bind(socket_UDP, (struct sockaddr *)&server_UDP, sizeof(server_UDP));
	if( bindStatus == -1){
		//print the error message
		perror("UDP Binding failed");
		return 1;
	}
    puts("UDP Socket is running now...\n");

    //Receive a message from client
    int recvStatus;
    char client_selection[10];

	while(1){
        recvStatus = recv(client_TCP, client_selection, 5000, 0);
        if (recvStatus == -1){
            printf("Error in receiving!");
            break;
        }
        else printf("Client selected: %s\n", client_selection);
        
        if (client_selection[0] == '1') {
            // recv - TCP 
            recvStatus = recv(client_TCP, client_nonvowel, 5000, 0);
            if (recvStatus==-1) {
                printf("Error in receiving!");
                break;
            }
            //else printf("TCP nonvowels: %s\n", client_nonvowel);
            // send - TCP
            if (send(client_TCP, client_nonvowel, strlen(client_nonvowel), 0) <0 ) {
                printf("TCP nonvowels Send Failed\n");
            } else printf("\nTCP nonvowels Send Success\n");

            // recv - UDP 
            struct sockaddr_in cliaddr;
            int n, len; 
            len = sizeof(cliaddr);

            n = recvfrom(socket_UDP, client_vowel, 5000,
                0, ( struct sockaddr *) &cliaddr, &len); 
            if (n < 0) {
                printf("UDP vowels recv Failed\n");
            }
            else {
                client_vowel[n] = '\0';
                //printf("UDP vowels: %s\n", client_vowel);
            }
            // send - UDP 
            if (sendto(socket_UDP, client_vowel,
                strlen(client_vowel), 0, (const struct sockaddr*)&cliaddr, sizeof(cliaddr)) <0) 
            {
                    puts("UDP send Failed");
            } 
            else printf("UDP vowels send success\n\n");

        } else if (client_selection[0] == '2'){
            //printf("this is SELECTION 2!\n");
            char client_envowel[5000];
            recvStatus = recv(client_TCP, client_envowel, 5000, 0);
            if (recvStatus==-1) {
                printf("Error in receiving!");
                break;
            }
            //else printf("TCP nonvowels: %s\n", client_nonvowel);
            // send - TCP
            if (send(client_TCP, client_envowel, strlen(client_envowel), 0) <0 ) {
                printf("TCP client_envowel Send Failed\n");
            } else printf("\nTCP client_envowel Send Success\n");
        } 
		
	}

    close(client_TCP);
    close(socket_UDP);
    close(socket_TCP);

	return 0;
}