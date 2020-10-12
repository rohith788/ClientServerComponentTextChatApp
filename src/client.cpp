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

bool compare_for_list_in_client(socket_info client1, socket_info client2)
{
    return client1.port_num < client1.port_num;
}

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
    int one = 1;
    setsockopt(system_socket.listner, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if (bind(system_socket.listner, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address)) == -1)
    {
        cerr << "bind\n";
        cerr << ("%dss\nvb", errno);
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
            int i = 1;
            system_socket.client_list.sort(compare_for_list_in_client);
            for (list<socket_info>::iterator client = system_socket.client_list.begin(); client != system_socket.client_list.end(); ++client)
            {
                cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i++, client->hostname, client->ip_addr, client->port_num);
            }
            cse4589_print_and_log("[LIST:END]\n");
        }
        else if (strncmp(buffer, "LOGIN", 5) == 0)
        {
            // cout << ("gere");
            strtok(buffer, " ");
            // cse4589_print_and_log("here %s\n", buffer);
            char *server_ip_address = strtok(NULL, " "); //Extract ip from input
            char *server_port_num = strtok(NULL, " ");   //extract port from input
            // cse4589_print_and_log("1 %s\n", buffer);
            // cse4589_print_and_log("2 %s\n", server_ip_address);
            // cse4589_print_and_log("3 %s\n", server_port_num);
            if (server_ip_address == NULL)
            {
                cse4589_print_and_log("[LOGIN:ERROR]\n");
                cse4589_print_and_log("NULL ip");
                cse4589_print_and_log("[LOGIN:END]\n");
                continue;
            }
            int port_valid = 0;
            for (int i = 0; i != strlen(server_port_num); ++i)
            {
                if (server_port_num[i] < '0' && server_port_num[i] > 9)
                {
                    cse4589_print_and_log("[LOGIN:ERROR]\n");
                    // cse4589_print_and_log("port");
                    // cerr << errno;
                    cse4589_print_and_log("[LOGIN:END]\n");
                    port_valid = 1;
                    break;
                }
            }
            if (port_valid == 1)
            {
                continue;
            }
            int atoi_port = atoi(server_port_num);
            if (atoi_port < 0 || atoi_port >= 65534)
            {
                cse4589_print_and_log("[LOGIN:ERROR]\n");
                // cse4589_print_and_log("port to atoi");
                cse4589_print_and_log("[LOGIN:END]\n");
                continue;
            }
            //https: //man7.org/linux/man-pages/man3/inet_pton.3.html
            //check ip validity
            struct sockaddr_in ip_addr_for_valid;
            ip_addr_for_valid.sin_family = AF_INET;
            ip_addr_for_valid.sin_port = htons(atoi_port);
            if (inet_pton(AF_INET, server_ip_address, &ip_addr_for_valid.sin_addr) != 1) // checking for ip validity
            {
                cse4589_print_and_log("[LOGIN:ERROR]\n");
                // cse4589_print_and_log("valid_ip");
                cse4589_print_and_log("[LOGIN:END]\n");
                continue;
            }
            struct addrinfo hints, *res;
            bzero(&hints, sizeof(hints));
            if ((getaddrinfo(server_ip_address, server_port_num, &hints, &res)) != 0)
            {
                cse4589_print_and_log("[LOGIN:ERROR]\n");
                // cse4589_print_and_log("get_addr");
                cse4589_print_and_log("[LOGIN:END]\n");
                continue;
            }
            system_socket.listner = socket(AF_INET, SOCK_STREAM, 0); //socket fd
            if (system_socket.listner < 0)
            {
                cse4589_print_and_log("[LOGIN:ERROR]\n");
                // cse4589_print_and_log("socket");
                cse4589_print_and_log("[LOGIN:END]\n");
                continue;
            }
            struct sockaddr_in serv_addr;
            bzero(&serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(atoi_port);
            serv_addr.sin_addr.s_addr = inet_addr(server_ip_address);
            //connection to server
            if ((connect(system_socket.listner, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr))) < 0)
            {
                cse4589_print_and_log("[LOGIN:ERROR]\n");
                // cerr << errno;
                cse4589_print_and_log("[LOGIN:END]\n");
                continue;
            }
            char client_port_num[8];
            bzero(client_port_num, sizeof(client_port_num));
            //sending port number to connected server
            if (send(system_socket.listner, system_socket.port_num, strlen(system_socket.port_num), 0) < 0)
            {
                cerr << "port\n";
                cerr << errno;
            }

            char message_buffer[1024]; // buffer to send to the connected server
            for (;;)
            {
            }
        }
    }
}