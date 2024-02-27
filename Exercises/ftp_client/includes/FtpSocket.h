//
// Created by marco on 25/02/2024.
//

#ifndef FTP_CLIENT_FTPSOCKET_H
#define FTP_CLIENT_FTPSOCKET_H

#include <string>
#include <winsock2.h>
#include <iostream>
#include <cstdint>
#include "FtpClient.h"

class FtpSocket {
private:

public:
    FtpSocket();

    ~FtpSocket();

    static int initWinSock() {
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData);
    };

    static int ftpServerConnect(SOCKET _controlSocket, const std::string &serverIP, uint16_t port) {
        struct sockaddr_in ftpServer{};

        // Set up the FTP server address
        ftpServer.sin_family = AF_INET;
        ftpServer.sin_port = htons(port);
        ftpServer.sin_addr.s_addr = inet_addr(serverIP.c_str());

        return connect(_controlSocket, (struct sockaddr *) &ftpServer, sizeof(ftpServer));
    }
};


#endif //FTP_CLIENT_FTPSOCKET_H
