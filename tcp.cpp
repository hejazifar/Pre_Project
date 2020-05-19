#include "tcp.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// create the server, if successful, return the clientsocket otherwise return a
// digit other than 0 based on the case
int tcp::tcp_server_init(int port, string ipaddress) {
  // Create a socket
  int listening = socket(AF_INET, SOCK_STREAM, 0);
  if (listening == -1) {
    cerr << "Can't create a socket" << endl;
    return -1;
  }
  // Bind the socket to IP / port
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  inet_pton(AF_INET, ipaddress.c_str(), &hint.sin_addr); // 127.0.0.1

  if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1) {
    cerr << "Can't bind to IP/port" << endl;
    return -2;
  }
  // Mark the socket for listening in
  if (listen(listening, SOMAXCONN) == -1) {
    cerr << "Can't listen!";
    return -3;
  }
  // Accept a call
  sockaddr_in client;
  socklen_t clientSize = sizeof(client);
  char host[NI_MAXHOST];
  char svc[NI_MAXSERV];

  int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

  if (clientSocket == -1) {
    cerr << "Problem with client connection";
    return -4;
  }

  // Close the listening socket

  close(listening);

  memset(host, 0, NI_MAXHOST);
  memset(svc, 0, NI_MAXSERV);

  int result = getnameinfo((sockaddr *)&client, sizeof(client), host,
                           NI_MAXHOST, svc, NI_MAXSERV, 0);
  if (result) {
    cout << host << " connected on " << svc << endl;
  } else {
    inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
    cout << host << "connected on" << ntohs(client.sin_port) << endl;
  }
  return clientSocket;
}

int tcp::tcp_client_init(int port, string ipaddress) {
  // Create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    return 1;
  }
  // Creat a hint structure for the server we're connection with

  sockaddr_in hint; // A.B.C.D type of address
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  inet_pton(AF_INET, ipaddress.c_str(),
            &hint.sin_addr); // convert ipaddress to a binary format and save it
                             // hint.sin.addrr

  // Connect to the server on the socket
  int connectResult = connect(sock, (sockaddr *)&hint, sizeof(hint));
  if (connectResult == -1) {
    return -1;
  }
  return sock;
}

// send the message, if successful return 0 otherwise return -1
int tcp::tcp_send(int sock, char *message, int size) {
  int sendRes = send(sock, message, size, 0);
  if (sendRes == -1) {
    cout << "Could not send the message! \r\n";
    return -1;
  }
  return 0;
}

// receive the message and return the data as a char array
char *tcp::tcp_receive(int sock, char *receivedData, int size) {
  int bytesRecv = recv(sock, receivedData, size, 0);
  if (bytesRecv == -1) {
    cerr << "There was a connection issue" << endl;
  }
  if (bytesRecv == 0) {
    cout << "The client disconnected" << endl;
  }
  // Display the message
  cout << "Recieved: " << receivedData << endl;
  return receivedData;
}

// close the socket
void tcp::tcp_close_socket(int sock) { close(sock); }