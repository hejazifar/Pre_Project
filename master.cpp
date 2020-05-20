#include "tcp.hpp"
#include <iostream>

using namespace std;
using namespace tcp;

int main() {
  int clientSock = 0;
  clientSock = tcp_server_init();

  char *sendData;
  sendData = new char[12];
  string message = "2+3= ";
  message.copy(sendData, message.size());
  tcp::tcp_send(clientSock, sendData, 12);

  char *recieveData;
  recieveData = new char[12];
  recieveData = tcp_receive(clientSock, recieveData, 12);
  cout << recieveData << endl;

  tcp_send(clientSock, sendData, 12);

  tcp_close_socket(clientSock);
}