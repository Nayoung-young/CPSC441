#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

#define SERVER_IP "127.0.0.1" 

#define PORT_TCP 9000
#define PORT_UDP 8888

int main(int argc, char *argv[]){

    printf("=============================\n");
    printf("Hello, this is vowelizer Server\n");
    printf("=============================\n\n");

    int socket_TCP, socket_UDP, client_TCP, client_UDP; 
    struct sockaddr_in server_TCP, server_UDP, 
        client_sock_UDP, client_sock_TCP; 
    char client_message[5000];

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
		perror("Binding failed!!");
		return 1;
	}

    listen(socket_TCP, 3);

    puts("TCP Socket is running now...");

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
		perror("Binding failed");
		return 1;
	}

    listen(socket_UDP, 3);

    puts("UDP Socket is running now...");

    // connect Client
    client_TCP = accept(socket_TCP, NULL, NULL);
	
    if (client_TCP < 0 ){
		perror("client_TCP Connection failed\n");
		return 1;
	}

    printf("Client Connection accepted...\n\n");

    //Receive a message from client
    int recvStatus;
	while(1){
        recvStatus = recv(client_TCP, client_message, 5000, 0);
        if (recvStatus==-1){
            printf("Error in receiving!");
            break;
        }

        printf("Client selected: %s\n", client_message);
		
        char serverMsg[5000];
        strcat(serverMsg,"Hello FROM NAYOUNG");
        // strcat(serverMsg, client_message);
		write(client_TCP , serverMsg , strlen(serverMsg)); // send msg to client
		write(client_UDP , serverMsg , strlen(serverMsg));
        // write something to your brower 
		
	}

    close(client_TCP);
    close(client_TCP);
    close(socket_UDP);
    close(socket_TCP);

	return 0;
}
