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

client::client(char *port)
{
    strcpy(system_socket.port_num, port); // get the port number
    struct hostent *host;                 //https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-hostent
    char host_name[1024];

    //https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/

    if (gethostname(host_name, sizeof(host_name)) < 0)
    {
        cerr << "gethostname\n";
        exit(1);
    }
    if ((host = gethostbyname(host_name)) == NULL)
    {
        cerr << "gethostname\n";
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
    } //bind the socket of the curent client system

    char buffer[1024];
    while (true)
    {
        bzero(buffer, sizeof(buffer));
        read(STDIN, buffer, 1024);
        buffer[strlen(buffer) - 1] = '\0';
        if (strcmp(buffer, "EXIT") == 0)
        {
            cse4589_print_and_log("[EXIT:SUCCESS]\n");
            cse4589_print_and_log("[EXIT:END]\n");
            break;
        }
        else if (strcmp(buffer, "AUTHOR") == 0)
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
        else if (strncmp(buffer, "LOGIN", 5) == 0)
        {
        }
    }
    cse4589_init_log("abc");
}