#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PORT 8000
#define SIZE 1024
int main(int argc, char const *argv[])
{
    struct stat info;

    char buffer[SIZE] = {0} , ACKbuf[SIZE] = {0} , LENbuf[SIZE] = {0};
    struct sockaddr_in address;
    int new_socket;
    int server_fd;
    int valread;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("Socket creation failed !!!");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)) != 0) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;         
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);       

    int check = bind(server_fd, (struct sockaddr *)&address,sizeof(address)); 
    if ( check < 0)
    {
        perror("bind error , failed !!");
        exit(EXIT_FAILURE);
    }
    int checklis = listen(server_fd, 3); 
    if ( checklis < 0) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int checksoc = new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t *)&addrlen);
    if ( checksoc < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        memset(buffer , '\0' , sizeof(buffer));
        valread = read(new_socket, buffer, SIZE);

        int fd = open(buffer, O_RDONLY);
        if (fd < 0)
        {
            write(new_socket, "File-not-found...", 17);
        	memset(ACKbuf , '\0' , sizeof(ACKbuf));

            valread = read(new_socket, ACKbuf, SIZE);
            write(new_socket, "Done!!", 6);
            continue;
        }
        stat(buffer, &info);
        int LEN;

        LEN = info.st_size;

        sprintf(LENbuf, "%d", LEN);
        
        write(new_socket, LENbuf, strlen(LENbuf));
        
        memset(ACKbuf , '\0' , sizeof(ACKbuf));

        
        valread = read(new_socket, ACKbuf, SIZE);
        
        int karli = LEN;
        while (karli >= SIZE)
        {
            char rray[1025];
        	memset(rray , '\0' , sizeof(rray));

            read(fd, rray, SIZE);
            write(new_socket, rray, SIZE);
            bzero(ACKbuf, SIZE);
        	memset(ACKbuf , '\0' , sizeof(ACKbuf));

            valread = read(new_socket, ACKbuf, 6);
            karli = karli - SIZE;
        }
        if (karli)
        {
            char rray[1025];
        	memset(rray , '\0' , sizeof(rray));

            read(fd, rray, karli);
            write(new_socket, rray, karli);
	        memset(ACKbuf , '\0' , sizeof(ACKbuf));

            valread = read(new_socket, ACKbuf, 6);
        }
        memset(ACKbuf , '\0' , sizeof(ACKbuf));
        write(new_socket, "IsItDone??", 10);
    }
    return 0;
}
