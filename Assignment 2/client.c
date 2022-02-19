#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

//#define SERVER_IP "127.0.0.1" 
#define SERVER_IP "136.159.5.27" 

#define ADVANCED 1
//#define ADVANCED 0

#define PORT_TCP 9000
#define PORT_UDP 8888

#define MAX_LENGTH 5000

int is_vowel(char c) {
    if (c == 'a'|| c =='A') 
        return 1;
    if (c == 'e'|| c =='E') 
        return 1;
    if (c == 'i'|| c =='I') 
        return 1;
    if (c == 'o'|| c =='O') 
        return 1;
    if (c == 'u'|| c =='U') 
        return 1;
    else 
        return 0;
}

int main(int argc , char *argv[])
{
	printf("=============================\n");
    printf("Hello, this is vowelizer Client\n");
    printf("=============================\n\n");

	int sock_UDP, sock_TCP;
	struct sockaddr_in server_TCP, server_UDP;
	char selection[2], message[1000] , 
		server_reply_UDP[2000], server_reply_TCP[2000];

	// create TCP 
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
			puts("\nYou selected 1");
			printf("Enter your message to devowel: ");
			//scanf("%[^\n]s", message);
			while(getchar()!='\n');
			fgets(message, sizeof(message), stdin);
			message[strlen(message)-1] ='\0';

			if ((int)strlen(message) < 1) {
				puts("please input message");
				return 1;
			}
			
			char vowel_UDP[5000], nonvowel_TCP[5000];
            char ch = message[0]; 
            int i = 0; 
            char blank = ' ';

			int a = 0;
			if (ADVANCED) {
				while (i < (int)strlen(message)) {
					// if non-vowel 
					if (!is_vowel(ch)){
						strncat(nonvowel_TCP, &ch, 1);
						//strncat(nonvowel_TCP, &blank, 1);
						a++;
					}
					// if vowel 
					else {
						char tmp = a+'0'; 
						strncat(vowel_UDP, &tmp, 1);
						strncat(vowel_UDP, &ch, 1);
						a = 0;
					}
					ch = message[++i];
				}
			} 
			else {
				while (i < (int)strlen(message)) {
					// if vowel 
					if (is_vowel(ch)){
						strncat(vowel_UDP, &ch, 1);
						strncat(nonvowel_TCP, &blank, 1);
					}
					// if non-vowel 
					else {
						strncat(nonvowel_TCP, &ch, 1);
						strncat(vowel_UDP, &blank, 1);
					}
					ch = message[++i];
				}
			}
			
            //printf("Vowelling is done!\n");
			//printf("nonvowels: %s, vowels: %s\n", nonvowel_TCP, vowel_UDP);

			// send vowels on UDP 
			int len = sizeof(server_UDP);
			if (sendto(sock_UDP, vowel_UDP, strlen(vowel_UDP),
        		0, (const struct sockaddr *) &server_UDP, sizeof(server_UDP))<0) 
			{
				puts("UDP Send failed");
				return 1;
			}
			//else puts("UDP Send success");

			// send nonvowels on TCP 
			if( send(sock_TCP , nonvowel_TCP , strlen(nonvowel_TCP) , 0) < 0)
			{
				puts("TCP Send failed");
				return 1;
			} 
			//else puts("TCP Send success");

			// receive from UDP
			int n = recvfrom(sock_UDP , server_reply_UDP , 5000
				, 0, (struct sockaddr *) &server_UDP, &len);
			if( n < 0) {
				puts("UDP recv failed");
				break;
			} else printf("\nServer sent     vowels on UDP: \'%s\'\n", server_reply_UDP);

			// receive from TCP
			if( recv(sock_TCP , server_reply_TCP , 5000 , 0) < 0)
			{
				puts("TCP recv failed");
				break;
			}
			else printf("Server sent non-vowels on TCP: \'%s\'\n\n", server_reply_TCP);
			
		} 
		else if (selection[0] == '2') {
			char nonvowel[1000], vowel[1000];

			puts("You selected 2");
			printf("Enter non-vowel part of message to envowel: ");
			while(getchar()!='\n');
			fgets(nonvowel, sizeof(nonvowel), stdin);
			nonvowel[strlen(nonvowel)-1] ='\0';

			printf("Enter     vowel part of message to envowel: ");
			//while(getchar()!='\n');
			fgets(vowel, sizeof(vowel), stdin);
			vowel[strlen(vowel)-1] ='\0';
			
			//printf("\nThis is nonvowel part input: \'%s\'\n", nonvowel);
			//printf("This is    vowel part input: \'%s\'\n", vowel);

			int k; 
			if (strlen(vowel) < strlen(nonvowel)) {
				k = strlen(vowel); 
				while (k < strlen(nonvowel)) {
					vowel[k++] = ' ';
				}
				vowel[k] ='\0';
			} else if (strlen(vowel) > strlen(nonvowel)) {
				k = strlen(nonvowel);
				while (k < strlen(vowel)) {
					nonvowel[k++] = ' ';
				}
				nonvowel[k] ='\0';
			}

			//printf("\nThis is nonvowel part input: \'%s\'\n", nonvowel);
			//printf("This is    vowel part input: \'%s\'\n", vowel);

			// envowel 
			int j = 0; // str idx 
			char blank = ' ';
			char ch_vowel = vowel[0];
			char ch_nonvowel = nonvowel[0];
			char envowel_msg[5000];

			puts("This is envowelizer Loop");
			while(j < strlen(vowel)) {
				// vowel[j] is blank
				//printf("this is j: %i\n", j);
				if (ch_vowel == blank){
					strncat(envowel_msg, &ch_nonvowel, 1);
					//puts("vowel[j] is blank");
				} else {
					strncat(envowel_msg, &ch_vowel, 1);
					//puts("vowel[j] is not blank");
				}
				//printf("This is envowel_msg now: %s\n", envowel_msg);
				ch_vowel = vowel[++j];
				ch_nonvowel = nonvowel[j];
			}
			printf("Envowel is done in client-side: %s\n", envowel_msg);

			// send to TCP
			if( send(sock_TCP , envowel_msg , strlen(envowel_msg) , 0) < 0)
			{
				puts("TCP Send failed");
				return 1;
			} 

			// recv from TCP
			if( recv(sock_TCP , server_reply_TCP , 5000 , 0) < 0)
			{
				puts("TCP recv failed");
				break;
			}
			else printf("Server sent envowel results on TCP: \'%s\'\n\n", server_reply_TCP);

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
		
		
	}
	
	close(sock_TCP);
	close(sock_UDP);
	
	return 0;
}