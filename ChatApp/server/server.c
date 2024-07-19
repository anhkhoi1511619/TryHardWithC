// Server side C program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#define PORT 51111
char *init_text(char *input);
void *read_data_socket_func();
void *send_data_socket_func();

int function = 0;
int new_socket;
char* text;
int main(int argc, char const* argv[])
{
    printf("Please choose a function:\n>>");
    scanf("%d", &function);

    int server_fd;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    text = init_text(text);
 
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    pthread_t read_data_socket;
    pthread_t send_data_socket;

    pthread_create(&read_data_socket, NULL, read_data_socket_func, NULL);
    pthread_create(&send_data_socket, NULL, send_data_socket_func, NULL);

    pthread_exit(NULL);

    // do
    // {
    //     if(strlen(text) != 0) {
    //     //     valread = read(new_socket, buffer,
    //     //            1024 - 1); // subtract 1 for the null
    //     //                       // terminator at the end
    //     // printf("Message received: %s\n", buffer);
    //         send(new_socket, text, strlen(text), 0);
    //         printf("Message sent: %s\n", text);
    //         free(text);
    //     } else{
    //         text = init_text(text);
    //     }
    // } while (function != -1);

 
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    return 0;
}

char *init_text(char *text) {
    text = (char *)malloc(100 * sizeof(char)); // Allocate memory for the input string
    if (text == NULL) {
        printf("Memory allocation failed\n");
    }

    printf("Please type message to send for client:\n>> ");
    scanf(" ");
    scanf("%99[^\n]s", text); // Use %99s to avoid buffer overflow
    printf("You entered: %s length: %lu\n", text, strlen(text));
    return text;
}

void* read_data_socket_func()
{
    do
    {
        char buffer[1024] = { 0 };
        int valread = read(new_socket, buffer,
                   1024 - 1); // subtract 1 for the null
                              // terminator at the end
        if(valread > 0) {
            printf("Message received: %s\n", buffer);
        } else{
            printf("Checking data from socket\n");
        }
    } while (function != -1);

}
void* send_data_socket_func()
{
    do
    {
        if(strlen(text) != 0) {
            send(new_socket, text, strlen(text), 0);
            printf("Message sent: %s\n", text);
            free(text);
        } else{
            text = init_text(text);
        }
    } while (function != -1);
}