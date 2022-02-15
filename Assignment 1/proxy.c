#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int main(int argc, char *argv[])
{
   // -- with client 
   // socket
   int socket_desc, client_sock;
	struct sockaddr_in server, client;
	char client_message[5000];

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

   // bind 
   int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
	if( bindStatus == -1){
		//print the error message
		perror("Binding failed");
		return 1;
	}
	printf("Binding done.\n");

   // listen
   listen(socket_desc, 3);

   // accept
   client_sock = accept(socket_desc, NULL, NULL);
   if (client_sock < 0){
		perror("Connection failed");
		return 1;
	}
	printf("Connection accepted\n");

   // recv 
   int recvStatus;
   while(1) {
       recvStatus = recv(client_sock, client_message, 5000, 0);
       if (recvStatus==-1) {
           printf("Error in receiving!");
           break;
       }

       printf("Brower Says: %s\n", client_message);


   }

   // send 
   // close 

   // -- with server 
   // socket 
   // connect 
   // send 
   // recv 
   // close 

	
}