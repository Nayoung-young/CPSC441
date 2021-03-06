#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int main(int argc, char *argv[])
{
	// create
    int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];

	server.sin_addr.s_addr = inet_addr("127.0.0.1"); // 내 서버일 경우 
	server.sin_family = AF_INET;
	server.sin_port = htons(9000); // port (server port랑 같아야 함)

	// connect 
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	// send and recv 
	while(1)
	{
		// printf("Enter message : ");
		// scanf("%s" , message);
		
		//Send some data
		// if( send(sock , message , strlen(message) , 0) < 0)
		// {
		// 	puts("Send failed");
		// 	return 1;
		// }
		
		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		// {	// change this code as you want 
		// 	puts("recv failed");
		// 	break;
		// }
		
		// puts("Server reply :");
		// puts(server_reply);
	}

	// close 
	close(sock);
	return 0; 
}