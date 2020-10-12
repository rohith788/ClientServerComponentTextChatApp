#ifndef GLOBAL_H_
#define GLOBAL_H_

#define HOSTNAME_LEN 128
#define PATH_LEN 256

#include <string.h>
#include <queue>
#include <list>

struct block
{
    char host[1024];
    char ip[1024];
    int port_number_listenting_to;
};
struct buffer_info
{
    char destination_ip[32];
    char message[1024];
    char fr[32];
};

struct socket_info
{
    int fd; //file descriptor
    int list_id;
    char hostname[40];
    char ip_addr[20];
    int port_num;
    int num_msg_sent; //number of message sent
    int num_msg_rcv;  //number of message recieved
    char status[16];
    std::list<socket_info> client_list;
    std::queue<buffer_info> buff;
};

struct system_info
{
    std::list<socket_info> client_list;
    std::list<block> block_list;
    char ip_addr[1024];
    char port_num[1024];
    int listner;
    int yes;
    int system_num;
};

class common
{
protected:
    system_info system_socket;
};

class client : public common
{
public:
    client(char *port);

    bool check_client_ip_valid(char *ip, int port);
    void break_command(const char *cmd, char *&server_ip, char *&server_port);
};

class server : public common
{
public:
    server(char *port);
    bool check_server_ip_valid(char *ip);
};

#endif
