#include "tcp.hpp"
#include <iostream>

using namespace std;
string message;

int calculate(string message);

int main() {

  int serverSock = 0;
  serverSock = tcp::tcp_client_init();

  // receiving data
  char *recieveData;
  recieveData = new char[12];
  recieveData = tcp::tcp_receive(serverSock, recieveData, 12);
  message = string(recieveData);

  // parsing data and calculating
  message = to_string(calculate(message));

  // sending back the result
  char *sendData;
  sendData = new char[12];
  message.copy(sendData, message.size());
  tcp::tcp_send(serverSock, sendData, 12);

  tcp::tcp_close_socket(serverSock);

  return 0;
}

int calculate(string message) {
  int num1 = stoi(message.substr(0, message.find("+")));
  int num2 = stoi(message.substr(message.find("+") + 1,
                                 message.find("=") - message.find("+") - 1));
  return num1 + num2;
}