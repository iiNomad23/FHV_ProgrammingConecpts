//
// Created by marco on 25/02/2024.
//

#ifndef FTP_CLIENT_FTPSOCKET_H
#define FTP_CLIENT_FTPSOCKET_H

#include <winsock.h>
#include <cstdint>
#include "exeptions/SocketConnectionFailureException.h"

constexpr static uint16_t CONTROL_RESPONSE_BUFFER_SIZE = 1024;

class FtpSocket {
private:
    SOCKET _socket;

    explicit FtpSocket(SOCKET socket);

public:
    ~FtpSocket();

    [[nodiscard]] std::string receiveResponse() const;

    int receiveFileData(char *buffer) const;

    void sendCommand(const std::string &request) const;

    void close();

    static FtpSocket createSocket(const std::string &serverIP, uint16_t port);
};


#endif //FTP_CLIENT_FTPSOCKET_H
