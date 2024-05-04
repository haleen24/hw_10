#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>
#include <errno.h>

#define SIZE 255ul

void dataSender(int sock) {
    std::string msg;
    std::string_view msg_view;
    for (;;) {
        getline(std::cin, msg);
        if (send(sock, msg.c_str(), msg.length(), 0) != std::min(msg.length(), SIZE)) {
            std::cout << "send different number of bytes\n";
        }
        if (msg == "The End") {
            close(sock);
            exit(0);
        }
    }
}

void dataReceiver(int sock) {
    char buff[SIZE];
    std::string msg;
    for (;;) {
        recv(sock, buff, SIZE, 0);
        msg = buff;
        std::cout << msg;
        if (msg == "The End") {
            close(sock);
            exit(0);
        }
    }
}

int main(int argc, char **argv) {
    int sock;
    if (argc != 3) {
        std::cout << "ARG FORMAT: <SERVER IP> <SERVER PORT>\n";
        exit(-1);
    }
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "socket() failed\n";
        exit(-1);
    }
    char *serverIp = argv[1];
    unsigned short serverPort = atoi(argv[2]);
    struct sockaddr_in serv{};
    memset(&serv, 0, sizeof(serv));
    serv.sin_port = htons(serverPort);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(serverIp);

    if (connect(sock, (struct sockaddr *) &serv, sizeof(serv)) < 0) {
        std::cout << "connection() failed\n";
        std::cout << errno;
        exit(-1);
    }
    char buff[SIZE + 1];
    recv(sock, buff, SIZE, 0);
    buff[SIZE] = '\0';
    std::string msg = buff;
    if (msg == "1") {
        dataSender(sock);
    } else if (msg == "2") {
        dataReceiver(sock);
    } else {
        std::cout << "Server response error\n";
        exit(-1);
    }

}
