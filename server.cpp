#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>

#define SIZE 255ul
#define MAXPENDING 5

void accept_client(int &clSock, int &servSock, struct sockaddr_in *client) {
    socklen_t clientSize = sizeof(*client);
    if ((clSock = accept(servSock, (struct sockaddr *) client, &clientSize)) < 0) {
        std::cout << "accept() failed\n";
        exit(-1);
    }
}

int main(int argc, char **argv) {
//    if (argc < 2 || argc > 3) {
//        std::cout << "ARG FORMAT: <SERVER IP> <SERVER PORT>\n";
//        exit(-1);
//    }
//    char *servIp = argv[1];
    unsigned short port = atoi(argv[1]);
    int servSock;
    struct sockaddr_in serv{};
    struct sockaddr_in clients[2];
    int clients_sock[2];
    memset(&serv, 0, sizeof(serv));
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = port;
    serv.sin_family = AF_INET;

    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "socket() failed\n";
        exit(-1);
    }

    if (bind(servSock, (struct sockaddr *) &serv, sizeof(serv)) < 0) {
        std::cout << "bind() failed\n";
        exit(-1);
    }
    std::cout << "Server on: " << inet_ntoa(serv.sin_addr) << ":" << port << '\n';
    if (listen(servSock, MAXPENDING) < 0) {
        std::cout << "listen() failed\n";
        exit(-1);
    } else {
        std::cout << "listening\n";
    }
    accept_client(clients_sock[0], servSock, &clients[0]);
    std::string msg = "1";
    send(clients_sock[0], msg.c_str(), msg.length(), 0);
    accept_client(clients_sock[1], servSock, &clients[1]);
    msg = "2";
    send(clients_sock[1], msg.c_str(), msg.length(), 0);

    char buff[SIZE];
    for (;;) {
        recv(servSock, buff, SIZE, 0);
        msg = buff;
        send(clients_sock[1], msg.c_str(), msg.length(), 0);
        if (msg == "The End") {
            close(servSock);
            exit(0);
        }
    }
}
