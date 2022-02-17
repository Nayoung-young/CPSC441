#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

int main(int argc, char *argv[]){

	int socket_desc, client_sock;
	struct sockaddr_in server;
	char client_message[5000];

	
	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket\n");
	}
	puts("Socket created\n");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(9000);
	
	//Bind
    int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
	if( bindStatus == -1){
		//print the error message
		perror("Binding failed");
		return 1;
	}
	printf("Binding done.\n");
	
	//Listen
	listen(socket_desc, 3);
	
	//Accept and incoming connection
	printf("Waiting for clients...\n");
	
	//accept connection from an incoming client
	client_sock = accept(socket_desc, NULL, NULL);
	
	if (client_sock < 0){
		perror("Connection failed");
		return 1;
	}
	printf("Connection accepted\n");
	
	//Receive a message from client
    int recvStatus;
	int count; 
	count = 0;
	while(1){
        recvStatus = recv(client_sock, client_message, 5000, 0);
        if (recvStatus==-1){
            printf("Error in receiving!");
            break;
        }

		count ++; 
		printf("Count is: %i\n", count);
		// 이 loop 안에서 계속 메시지 recv 
        printf("Client says: %s\n", client_message);
		char serverMsg[5000];
        strcat(serverMsg,"Hi Client, this is your msg: ");
        strcat(serverMsg, client_message);
		write(client_sock , serverMsg , strlen(serverMsg));
	}

    close(client_sock);
    close(socket_desc);

	return 0;
}
