#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PORT 8000
#define SIZE 1024
char exit_f[14];
char get_f[14];

int main(int argc, char const *argv[])
{
    char buffer[SIZE] , ackbuffer[SIZE] , printbuffer[SIZE] , VALbuf[1025];
    struct sockaddr_in address ;
    struct sockaddr_in serv_addr;
    int valread;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("\n Socket creation error !!\n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    int convip = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); 
    if ( convip < 1)
    {
        printf("\nInvalid address/ Address not found/supported \n");
        return -1;
    }

    int checkcon = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 
    if ( checkcon < 0) 
    {
        printf("\nConnection Failed !!\n");
        return -1;
    }

    while (1)
    {
        char input[SIZE];
        char compinp[SIZE][SIZE];
        int sz;
        strcpy(get_f, "get");
        strcpy(exit_f, "exit");
        int i = 0 , n_of_files = 0;
        printf("To be imported >");
        if (fgets(input, SIZE, stdin) == 0)
            break;
        
        sz = strlen(input);
        if (input[sz - 1] == '\n')
            input[sz - 1] = '\0';

        char *token = strtok(input, " ");
        while (token != NULL)
        {
            strcpy(compinp[i], token);
            i++;n_of_files++;
            token = strtok(NULL, " ");
        }
        

        int a;
        if (strncmp(compinp[0], get_f, 3) == 0)
        {
            for (a = 1; a <= n_of_files - 1; a++)
            {
                int xx = write(sock, compinp[a], strlen(compinp[a]));
                memset(buffer, '\0' , sizeof(buffer));
                valread = read(sock, buffer, SIZE);
                if (!strcmp(buffer, "File-not-found..."))
                {
                    printf("File not found !!\n");
                    write(sock, "okay", 4);
	                memset(ackbuffer, '\0' , sizeof(ackbuffer));
                    valread = read(sock, ackbuffer, SIZE);
                    continue;
                }
                printf("File found....\n");
                int LEN;
                double perc;
                int fd , checkkkk;
                LEN = atoi(buffer);
                write(sock, "okay", 4);
                checkkkk = LEN;
                fd = open(compinp[a], O_CREAT | O_RDWR | O_APPEND, 0600);
                while (checkkkk >= SIZE)
                {
                    perc = 0;
                    memset(VALbuf, '\0', sizeof(VALbuf));

                    valread = read(sock, VALbuf, SIZE);
                    write(fd, VALbuf, SIZE);
                    write(sock, "milgya", 6);
                    checkkkk -= SIZE;
                    perc = (double)(LEN - checkkkk); 
                    perc *= 100;
                    perc /= LEN;
                    printf("%0.2lf percent\r", perc);
                }
                if (checkkkk)
                {
                    perc = 0;
                    memset(VALbuf,'\0',sizeof(VALbuf));
                    valread = read(sock, VALbuf, checkkkk);
                    write(fd, VALbuf, checkkkk);
                    write(sock, "milgya", 6);
                }
                    memset(ackbuffer,'\0',sizeof(ackbuffer));
                valread = read(sock, ackbuffer, SIZE);
                printf("file %s done   \n", compinp[a]);
            }
        }
        else
        {
	        if (strncmp(compinp[0], exit_f, 4) == 0)
   	        	 exit(1);
   		 	else
       		 	printf("Wrong command !!\n");
        }


    }
    return 0;
}
