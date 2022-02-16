#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

#define SERVER_IP "127.0.0.1" 

#define PORT_TCP 9000
#define PORT_UDP 8888

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
    char client_selection[2];

	while(1){
        recvStatus = recv(client_TCP, client_selection, 5000, 0);
        if (recvStatus==-1){
            printf("Error in receiving!");
            break;
        }
        printf("Client selected: %s\n", client_selection);

        if (client_selection[0] == '1') {
            // devowel 
            recvStatus = recv(client_TCP, client_message, 5000, 0);
            if (recvStatus==-1) {
                printf("Error in receiving!");
                break;
            }
            printf("String to Devowel: %s\n", client_message);

            char vowel_TCP[1000], nonvowel_UDP[1000];
            char ch = client_message[0]; 
            int i = 0; 
            char blank[5] = " ";

            while (i < (int)strlen(client_message)) {
                // if vowel 
                if (vowel_check(ch)){
                    strncat(vowel_TCP, &ch, 1);
                    strcat(nonvowel_UDP, blank);
                }
                // if non-vowel 
                else {
                    strncat(nonvowel_UDP, &ch, 1);
                    strcat(vowel_TCP, blank);
                }
                ch = client_message[++i];
            }
            write(client_TCP, vowel_TCP, strlen(vowel_TCP));
            write(client_UDP, nonvowel_UDP, strlen(nonvowel_UDP));
        }
        else if (client_selection[1] == '2'){
            printf("~~\n");
        } else {
            break;
        }

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
