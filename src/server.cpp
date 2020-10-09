#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include "../include/logger.h"
#include "../include/global.h"

#define STDIN 0
using namespace std;

server::server(char *port)
{
    strcpy(system_socket.port_num, port); // get the port number
    struct hostent *host;                 //https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-hostent
    char host_name[1024];

    //https://www.geeksforgeeks.org/c-program-display-_-ip-address/

    if (gethostname(host_name, sizeof(host_name)) < 0)
    {
        cerr << "get_\n";
        exit(1);
    }
    if ((host = gethostbyname(host_name)) == NULL)
    {
        cerr << "get_\n";
        exit(1);
    }
    char *IPbuffer;
    IPbuffer = inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
    strcpy(system_socket.ip_addr, IPbuffer); // get the IP address

    system_socket.listner = socket(AF_INET, SOCK_STREAM, 0);
    if (system_socket.listner < 0)
    {
        cerr << "socket\n";
        exit(1);
    } // create the socket

    struct sockaddr_in client_socket_address;
    bzero(&client_socket_address, sizeof(client_socket_address));
    client_socket_address.sin_family = AF_INET;
    client_socket_address.sin_addr = *((struct in_addr *)host->h_addr);
    client_socket_address.sin_port = htons(atoi(port));
    if (bind(system_socket.listner, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address) < 0))
    {
        cerr << "bind\n";
        exit(1);
    } //bind the socket of the server system

    if (listen(system_socket.listner, 10) < 0) //setting limit to 10
    {
        cerr << "listen";
        exit(1);
    }

    fd_set master_list;
    fd_set watch_list;
    FD_ZERO(&master_list);
    FD_ZERO(&watch_list);
    FD_SET(system_socket.listner, &master_list);
    FD_SET(STDIN, &master_list);
    //adding listners to master list file descriptor

    int address_length;
    int file_descriptor;
    int nbytes;
    int max_fd = system_socket.listner;
    struct sockaddr_in remote_client_address;
    char buffer[1024];
    while (true)
    {
        watch_list = master_list;
        int ret;
        if ((ret = select(max_fd + 1, &watch_list, NULL, NULL, NULL)) < 0) //moniters multiple connections Synchronously
        {
            cerr << "error\n";
            exit(1);
        }

        for (int i = 0; i <= max_fd; i++)
        {
            memset((void *)&buffer, '\0', sizeof(buffer));
            if (FD_ISSET(i, &watch_list)) //checks if the fd is in the set
            {
                if (i == STDIN)
                {
                    read(STDIN, buffer, 1024);
                    buffer[strlen(buffer) - 1] = '\0';
                    if (strcmp(buffer, "AUTHOR") == 0)
                    {
                        cse4589_print_and_log("[AUTHOR:SUCCESS]\n");
                        cse4589_print_and_log("I, rohithsu, have read and understood the course academic integrity policy.\n");
                        cse4589_print_and_log("[AUTHOR:END]\n");
                    }
                    else if (strcmp(buffer, "IP") == 0)
                    {
                        cse4589_print_and_log("[IP:SUCCESS]\n");
                        cse4589_print_and_log("IP:%s\n", system_socket.ip_addr);
                        cse4589_print_and_log("[IP:END]\n");
                    }
                    else if (strcmp(buffer, "PORT") == 0)
                    {
                        cse4589_print_and_log("[PORT:SUCCESS]\n");
                        cse4589_print_and_log("PORT:%s\n", system_socket.port_num);
                        cse4589_print_and_log("[PORT:END]\n");
                    }
                    else if (strcmp(buffer, "LIST") == 0)
                    {
                        cse4589_print_and_log("[LIST:SUCCESS]\n");
                        cse4589_print_and_log("[LIST:END]\n");
                    }
                }
            }
        }
    }
}