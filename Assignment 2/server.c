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

int vowel_check(char c) {
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

int main(int argc, char *argv[]){

    printf("=============================\n");
    printf("Hello, this is vowelizer Server\n");
    printf("=============================\n\n");

    int socket_TCP, socket_UDP, client_TCP, client_UDP; 
    struct sockaddr_in server_TCP, server_UDP;
        //client_sock_UDP, client_sock_TCP; 
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
    puts("UDP Socket is running now...");

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
            // devowel 
            recvStatus = recv(client_TCP, client_message, 5000, 0);
            if (recvStatus==-1) {
                printf("Error in receiving!");
                break;
            }
            printf("String to Devowel: %s\n", client_message);

            char vowel_UDP[5000], nonvowel_TCP[5000];
            char ch = client_message[0]; 
            int i = 0; 
            char blank[2] = " ";

            while (i < (int)strlen(client_message)) {
                // if vowel 
                if (vowel_check(ch)){
                    strncat(vowel_UDP, &ch, 1);
                    strncat(nonvowel_TCP, &blank[0], 1);
                }
                // if non-vowel 
                else {
                    strncat(nonvowel_TCP, &ch, 1);
                    strncat(vowel_UDP, &blank[0], 1);
                }
                ch = client_message[++i];
            }
            printf("Vowelling is done!\n");
            
            //send(client_TCP , nonvowel_TCP , strlen(nonvowel_TCP) , 0);
            //send(client_UDP , vowel_UDP , strlen(vowel_UDP) , 0);
            send(client_TCP, nonvowel_TCP, strlen(nonvowel_TCP));

            struct sockaddr *client; 
            struct sockaddr_in ip_client;
            client = (struct sockaddr *)&ip_client; 
            
            if (sendto(client_UDP, vowel_UDP,
                strlen(vowel_UDP), 0, client, sizeof(client))==-1) {
                    puts("UDP send Failed");
            }
        }
        else if (client_selection[1] == '2'){
            printf("~~\n");
        } 
        else break;
        // char serverMsg[5000];
        // strcat(serverMsg,"Hello FROM NAYOUNG");
        // // strcat(serverMsg, client_message);
		// write(client_TCP , serverMsg , strlen(serverMsg)); // send msg to client
		// write(client_UDP , serverMsg , strlen(serverMsg));
        // // write something to your brower 
		
	}

    close(client_TCP);
    close(client_TCP);
    close(socket_UDP);
    close(socket_TCP);

	return 0;
}