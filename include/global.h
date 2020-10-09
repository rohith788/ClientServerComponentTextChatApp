#ifndef GLOBAL_H_
#define GLOBAL_H_

#define HOSTNAME_LEN 128
#define PATH_LEN 256

#include <string.h>
#include <queue>
#include <list>

struct system_info
{
    char ip_addr[1024];
    char port_num[1024];
    int listner;
    int yes;
    int system_num;
};

struct socket_info
{
    char hostname[40];
    char ip_addr[20];
    int port_num;
    int num_msg_sent;
    int num_msg_rcv;
    char status[16];
};

class common
{
protected:
    system_info system_socket;

public:
    void author();
    void ip();
    void port();
};

class client : public common
{
public:
    client(char *port);
};

class server : public common
{
public:
    server(char *port);
};

#endif
