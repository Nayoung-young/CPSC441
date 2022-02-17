#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

#define SERVER_IP "127.0.0.1" 

#define PORT_TCP 9000
#define PORT_UDP 8888

int main(int argc , char *argv[])
{
	printf("=============================\n");
    printf("Hello, this is vowelizer Client\n");
    printf("=============================\n\n");

	int sock_UDP, sock_TCP;
	struct sockaddr_in server_TCP, server_UDP;
	char selection[2], message[1000] , 
		server_reply_UDP[2000], server_reply_TCP[2000];

	// crate TCP 
	sock_TCP = socket(AF_INET , SOCK_STREAM , 0);
	if (sock_TCP == -1)
	{
		printf("Could not create socket");
	}

	server_TCP.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_TCP.sin_family = AF_INET;
	server_TCP.sin_port = htons(PORT_TCP);

	if (connect(sock_TCP , (struct sockaddr *)&server_TCP , sizeof(server_TCP)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	// create UDP 
    sock_UDP = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_UDP == -1)
	{
		printf("Could not create socket\n");
	}

	server_UDP.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_UDP.sin_family = AF_INET;
	server_UDP.sin_port = htons(PORT_UDP);

	puts("TCP Socket Connected");
	puts("UDP Socket Ready\n");
	
	//keep communicating with server
	while(1)
	{
		printf("Please choose from the following selections:\n");
		printf("	1 - Devowel a message\n");
		printf("	2 - Envowel a message\n");
		printf("	3 - Exit program\n");
		printf("Your desired menu selection?: ");
		scanf("%s", selection);

		// send selection -- TCP
		if( send(sock_TCP , selection , strlen(selection) , 0) < 0)
		{
			puts("Selection Send failed");
			return 1;
		} else {
			//puts("Selection Send success");
		}

		if (selection[0]=='1') {
			//printf("You are in selection[0] == i!\n");
			puts("You selected 1");
			printf("Enter your message to devowel: ");
			//fgets(message, 1000,stdin);
			//scanf("%[^\n]s", message);
			scanf("%s[^\n]", message);

			if ((int)strlen(message) < 1) {
				puts("please input message");
				return 1;
			}
			// send message
			if( send(sock_TCP , message , strlen(message) , 0) < 0)
			{
				puts("Send failed");
				return 1;
			} 
			else puts("TCP Send success");

			// receive from TCP
			if( recv(sock_TCP , server_reply_TCP , 5000 , 0) < 0)
			{
				puts("TCP recv failed");
				break;
			}

			// receive from UDP
			int len;
			if( recvfrom(sock_UDP , server_reply_UDP , strlen(server_reply_UDP)
				, 0, (struct sockaddr *) &server_UDP, (socklen_t *)&len) < 0)
			{
				puts("UDP recv failed");
				break;
			}

			printf("Server sent %zu bytes of non-vowels on TCP: %s\n", strlen(server_reply_TCP), 
				server_reply_TCP);

			printf("Server sent %zu bytes of vowels on UDP: %s\n", strlen(server_reply_UDP),
				server_reply_UDP);
			
		} 
		else if (selection[0] == '2') {
			puts("You selected 2");
			// send message
		} 
		else if (selection[0] == '3') {
			puts("Program exit");
			break;
		} 
		else {
			puts("you have to make a selection in 1, 2, 3\n"); 
			puts("Program exit");
			break; 
		}
		
		// if (selection[0]=='1') {
		// 	printf("You selected 1\n");
		// 	printf("Enter your message to devowel: ");
		// 	scanf("%[^\n]s", message);

		// 	// send message
		// 	if( send(sock_TCP , message , strlen(message) , 0) < 0)
		// 	{
		// 		puts("Send failed");
		// 		return 1;
		// 	} 
		// 	else {
		// 		puts("Words to devowel Sent!");
		// 	}

		// 	// receive from server
		// 	if( recv(sock_TCP , server_reply_TCP , 2000 , 0) < 0)
		// 	{
		// 		puts("TCP recv failed");
		// 		break;
		// 	}
		// 	if( recv(sock_UDP , server_reply_UDP , 2000 , 0) < 0)
		// 	{
		// 		puts("UDP recv failed");
		// 		break;
		// 	}

		// 	printf("Server sent %zu bytes of non-vowels on TCP: ", strlen(server_reply_TCP));
		// 	puts(server_reply_TCP);

		// 	printf("Server sent %zu bytes of vowels on UDP: ", strlen(server_reply_UDP));
		// 	puts(server_reply_UDP);
			
		// } 
		// else if (selection[0] == '2') {
		// 	puts("You selected 2\n");
		// 	// send message
		// } 
		// else if (selection[0] == '3') {
		// 	puts("Program exit\n");
		// 	break;
		// } 
		// else {
		// 	puts("you have to make a selection in 1, 2, 3\n"); 
		// 	puts("Program exit\n");
		// 	break; 
		// }
		
		//Send some data
		// if( send(sock , message , strlen(message) , 0) < 0)
		// {
		// 	puts("Send failed");
		// 	return 1;
		// }
		
		// //Receive a reply from the server
		// if( recv(sock , server_reply , 2000 , 0) < 0)
		// {
		// 	puts("recv failed");
		// 	break;
		// }
		
		// puts("Server reply :");
		// puts(server_reply);
	}
	
	close(sock_TCP);
	close(sock_UDP);
	
	return 0;
}