//
// Created by marco on 25/02/2024.
//

#ifndef FTP_CLIENT_FTPSOCKET_H
#define FTP_CLIENT_FTPSOCKET_H

#include <winsock.h>
#include <cstdint>
#include "exeptions/SocketConnectionFailureException.h"

constexpr static uint16_t CONTROL_RESPONSE_BUFFER_SIZE = 1024;
constexpr static uint16_t FILE_BUFFER_SIZE = 4096;
constexpr static uint8_t PROGRESS_BAR_UPDATE_INTERVAL_MS = 100;
constexpr static uint8_t PROGRESS_BAR_WIDTH = 50;

class FtpSocket {
private:
    SOCKET _socket;

    explicit FtpSocket(SOCKET socket);

    int receiveFileDataChunk(char *buffer) const;

    static void displayProgress(size_t received, size_t total);

public:
    ~FtpSocket();

    [[nodiscard]] std::string receiveResponse() const;

    void receiveFileData(const std::string &fileName, size_t fileSize);

    void sendCommand(const std::string &request) const;

    void close();

    static FtpSocket createSocket(const std::string &serverIP, uint16_t port);
};


#endif //FTP_CLIENT_FTPSOCKET_H
