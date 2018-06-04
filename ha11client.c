#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int menu(void);
void option(int opt, char *buffer);
void list(char *buffer);
void touch(char *buffer);
void mkdir(char *buffer);
void copy(char *buffer);
void del(char *buffer);


int main(int argc , char *argv[])
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    int socket_desc, portn;

    int opt;

    FILE *fp;
    char path[60];
    char string[2048];

    //system(buffer);
    fp = popen("hostname", "r");
    strcpy(string,"\n");
    while (fgets(path, 60, fp) != NULL)
    {
        strcat(string,path);
    }
    pclose(fp);
    printf("Here is the message: %s",string);


    portn = 9000;

    printf("\n ## Use CTRL + C para encerrar ##\n\n");

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        perror("Could not create socket");
        return -1;
    }
    printf("Socket Created\n");

    //Prepare the sockaddr_in structure
    server = gethostbyname("localhost");  //change here to work outside the VM
/*  server.sin_addr.s_addr = inet_addr("127.0.0.1");
 *  server.sin_family = AF_INET;
 *  server.sin_port = htons( 8888 );
 */

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portn);

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
    {
        perror("Connect error");
        close(socket_desc);
        return -1;
    }
    printf("\nConnected\n");


    while(1)
    {

        strcpy(buffer, "");
        opt = menu();
        option(opt, buffer); // Função seleciona a ação e passa o endereço de buffer

        //Send some msg
        if(send(socket_desc, buffer, 256, 0) < 0)
        {
            perror("Send failed");
            close(socket_desc);
            return -1;
        }


        //Receive reply
        bzero(buffer,256);
        if( recv(socket_desc, buffer, 255, 0) < 0)
        {
            perror("Receive failed");
            close(socket_desc);
            return -1;
        }
        printf("Reply received: ");
        printf("%s\n",buffer);
        //close(socket_desc);
    }

    return 0;

}

int menu(void)
{
    int opt;

    printf("\n\n\n\n\n\n Menu\n 1 listar \n 2 criar diretório\n 3 criar arquivo\n 4 copiar\n 5 apagar\n");
    printf("Write a option: ");
    scanf("%d", &opt);

    return opt;
}

void option(int opt, char *buffer)
{
    switch (opt)
    {
        //clrscr(); // a função clrscr(); é para limpar a tela
        case 1:
            list(buffer);
        break;

        case 2:
            mkdir(buffer);
        break;

        case 3:
            touch(buffer);
        break;

        case 4:
            copy(buffer);
        break;

        case 5:
            del(buffer);
        break;

        default :
            printf ("Valor invalido!\n");
    }
    printf("\n \n");
}

void list(char *buffer)
{
    strcpy(buffer, "ls -la");
}

void touch(char *buffer)
{
    char arg0[256];

    printf("\n--> insira o nome do arquivo\n");
    fgets(arg0, 256, stdin);

    strcpy(buffer, "touch ");
    strcat(buffer, arg0);
}

void mkdir(char *buffer)
{
    char arg0[256];

    printf("\n--> insira o nome do diretorio\n");
    fgets(arg0,256,stdin);

    strcpy(buffer, "mkdir ");
    strcat(buffer, arg0);
}

void copy(char *buffer)
{
    char arg0[256], arg1[256];

    printf("\n--> insira o nome/addr do arquivo/pasta a ser copiado\n");
    fgets(arg0,256,stdin);

    strcpy(buffer, "rm ");
    strcat(buffer, arg0);

    printf("\n--> insira o addr de destino\n");
    fgets(arg0,256,stdin);

    strcat(buffer, arg1);
}

void del(char *buffer)
{
    char arg0[256];

    printf("\n--> insira o nome/addr do arquivo/pasta a ser removido\n");
    fgets(arg0,256,stdin);

    strcpy(buffer, "rm ");
    strcat(buffer, arg0);
}
