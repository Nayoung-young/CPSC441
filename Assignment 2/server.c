#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

int main(int argc, char *argv[]){

    int socket_TCP, socket_UDP, client_TCP, client_UDP; 
    struct sockaddr_in server_TCP, server_UDP, client; 
    char client_message[5000];

    // create TCP 
    socket_TCP = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
	{
		printf("Could not create socket\n");
	}

    server_TCP.sin_family = AF_INET;
	server_TCP.sin_addr.s_addr = INADDR_ANY;
	server_TCP.sin_port = htons(9000);

    int bindStatus = bind(socket_TCP, (struct sockaddr *)&server_TCP, sizeof(server_TCP));
	if( bindStatus == -1){
		//print the error message
		perror("Binding failed");
		return 1;
	}

    listen(socket_TCP, 3);

    puts("TCP Socket is running now... \n");

    // crate UDP 

	//~~

    client_TCP = accept(socket_TCP, NULL, NULL);
    client_UDP = accept(socket_UDP, NULL, NULL);
	
    if (client_TCP < 0 ){
		perror("client_TCP Connection failed\n");
		return 1;
	}
    if (client_UDP < 0) {
        perror("client_UDP Connection failed\n");
        return 1;
    }

    printf("Client Connection accepted...\n");
	
	// //Receive a message from client
    // int recvStatus;
    // int i = 0;
	// while(1){
    //     recvStatus = recv(client_sock, client_message, 5000, 0);
    //     if (recvStatus==-1){
    //         printf("Error in receiving!");
    //         break;
    //     }

    //     // revStatus != -1, receive success 
    //     printf("Client says: %s\n", client_message);

    //     // 1. get ASCII text 
    //     char serverMsg[5000];
    //     strcat(serverMsg, client_message);
	// 	write(client_sock , serverMsg , strlen(serverMsg));
	// }

	// // 2. devoweling
    // char vowel[500]; 
    // char nonvowel[500]; 

    // // 3. envoweling 

    close(client_UDP);
    close(client_TCP);
    close(socket_UDP);
    close(socket_TCP);

	
	return 0;
}
