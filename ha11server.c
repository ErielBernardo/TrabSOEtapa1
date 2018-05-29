#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c, portn;
    struct sockaddr_in server , client;
    char buffer[256];
    char **in=NULL;
    int count=0;
    portn = 9000;

    int opt;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        perror("Could not create socket\n");
        return -1;
    }
    printf("Socket created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portn);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Bind failed");
        close(socket_desc);
        return -1;
    }
    printf("Bind done\n");


    //Listen
    listen(socket_desc , 3);

    //Accept incoming connection
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (new_socket<0)
    {
        perror("Connection rejected");
        close(socket_desc);
        return -1;
    }
    printf("Connection accepted\n");

    while(1)
    {
        //Read the message from socket
        if (read(new_socket,buffer,255) < 0)
        {
            perror("ERROR reading from socket");
            close(socket_desc);
            return -1;
        }

        //option(opt);
        //execl("/bin", buffer, NULL);
        system(buffer);
        printf("\nHere is the message: %s", buffer);

        if (write(new_socket,"Message received",16) < 0)
        {
            perror("ERROR writing to socket");
            close(socket_desc);
            return -1;
        }
    }

    close(new_socket);
    close(socket_desc);

    return 0;
}



