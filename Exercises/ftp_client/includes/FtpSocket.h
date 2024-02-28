//
// Created by marco on 25/02/2024.
//

#ifndef FTP_CLIENT_FTPSOCKET_H
#define FTP_CLIENT_FTPSOCKET_H

#include <winsock.h>
#include <cstdint>
#include "exeptions/SocketConnectionFailureException.h"

class FtpSocket {
private:
    SOCKET _socket;

public:
    explicit FtpSocket(SOCKET socket);
    ~FtpSocket();

    static int initWinSock() {
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData);
    };

    static FtpSocket createSocket(const std::string &serverIP, uint16_t port) {
        SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        struct sockaddr_in ftpServer{};
        ftpServer.sin_family = AF_INET;
        ftpServer.sin_port = htons(port);
        ftpServer.sin_addr.s_addr = inet_addr(serverIP.c_str());

        int responseCode = connect(newSocket, (struct sockaddr *) &ftpServer, sizeof(ftpServer));
        if (responseCode != 0) {
            throw SocketConnectionFailureException("Socket connection to server failed!");
        }

        return FtpSocket(newSocket);
    }

    [[nodiscard]] std::string receiveResponse() const;

    int receiveFileData(char* buffer) const;

    void sendCommand(const std::string &request) const;

    void close();
};


#endif //FTP_CLIENT_FTPSOCKET_H
