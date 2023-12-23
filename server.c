// // Server side C/C++ program to demonstrate Socket
// // programming
// #include <netinet/in.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #define PORT 8080
// int main(int argc, char const* argv[])
// {
// 	int server_fd, new_socket;
// 	ssize_t valread;
// 	struct sockaddr_in address;
// 	socklen_t addrlen = sizeof(address);
// 	char buffer[1024] = { 0 };
// 	char* hello = "Hello from server";

	
// // Creating socket file descriptor
// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
// 		perror("socket failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(PORT);

// 	// Forcefully attaching socket to the port 8080
// 	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
// 		perror("bind failed");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (listen(server_fd, 10) < 0) {
// 		perror("listen");
// 		exit(EXIT_FAILURE);
// 	}
// 	while(1)
// 	{
// 		if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
// 			perror("accept");
// 			exit(EXIT_FAILURE);
// 		}
//         char buffer[30000] = {0};
// 		valread = read(new_socket, buffer, 1024 - 1); // subtract 1 for the null
// 								// terminator at the end
// 		printf("%s\n", buffer);

// 		send(new_socket, hello, strlen(hello), 0);

// 		printf("Hello message sent\n");

// 		// closing the connected socket
// 		close(new_socket);
// 		if (!strcmp(buffer, "haha"))
// 			break;
// 	}
// 	// closing the listening socket
// 	close(server_fd);
// 	return 0;
// }


//#----------------------------------

// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int fd;
    // char *hello = "Hello from server";
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 243\n\n";
	if ((fd = open("index.html", O_RDONLY)) < 0)
	{
        perror("open :");
        exit(EXIT_FAILURE);
    }
	char str[3000] = {0};
	read(fd, str, 3000);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    int reuseAddr = 1;
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) == -1) {
    printf("Error setting socket option SO_REUSEADDR.\n");
    close(server_fd);
    return -1;
}

    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);

        printf("%s\n",buffer );

        write(new_socket , hello , strlen(hello));
        write(new_socket , str , strlen(str));
		
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}




//#################################pragma endregion


// Server side C program to demonstrate HTTP Server programming
// #include <stdio.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>

// #define PORT 8080
// int main(int argc, char const *argv[])
// {
//     int server_fd, new_socket; long valread;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
    
//     // Only this line has been changed. Everything is same.
//     char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 15\n\nHello world!";
    
//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//     {
//         perror("In socket");
//         exit(EXIT_FAILURE);
//     }
    

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons( PORT );
    
//     memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
//     {
//         perror("In bind");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, 10) < 0)
//     {
//         perror("In listen");
//         exit(EXIT_FAILURE);
//     }
//     while(1)
//     {
//         printf("\n+++++++ Waiting for new connection ++++++++\n\n");
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
//         {
//             perror("In accept");
//             exit(EXIT_FAILURE);
//         }
        
//         char buffer[30000] = {0};
//         valread = read( new_socket , buffer, 30000);
//         printf("%s\n",buffer );
//         write(new_socket , hello , strlen(hello));
//         printf("------------------Hello message sent-------------------");
//         close(new_socket);
//     }
//     return 0;
// }