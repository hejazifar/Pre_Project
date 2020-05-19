#ifndef TCP_HPP_INCLUDED
#define TCP_HPP_INCLUDED

#include <string>

namespace tcp{
    int tcp_server_init(int port = 8080, std::string ipaddress = "0.0.0.0");
    int tcp_client_init(int port = 8080, std::string ipaddress = "127.0.0.1");
    int tcp_send(int sock, char * message, int size);
    char * tcp_receive(int sock, char * recieveData, int size);
    void tcp_close_socket(int sock);
}

#endif