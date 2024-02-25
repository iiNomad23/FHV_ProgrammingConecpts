//
// Created by marco on 25/02/2024.
//

#ifndef FTP_CLIENT_FTPCLIENT_H
#define FTP_CLIENT_FTPCLIENT_H

#include "MySocket.h"
#include <string>
#include <iostream>
#include <limits>

class FTPClient {
private:
    bool _connected;
    SOCKET _controlSocket;
    SOCKET _dataSocket;

public:
    FTPClient(const std::string& serverIP, unsigned short port);
    ~FTPClient();

    [[nodiscard]] bool isConnected() const;

    void login(const std::string& userName, const std::string& password) const;

    static std::string receiveResponse(SOCKET sock) {
        char buffer[1024] = {};
        recv(sock, buffer, sizeof(buffer), 0);
        std::string response(buffer);
        std::cout << "Received: " << response;
        return response;
    };

    static void sendCommand(SOCKET sock, const std::string& cmd) {
        std::cout << "Sent: " << cmd;

        // ensure the command length does not exceed the maximum value for int
        if (cmd.length() > static_cast<std::string::size_type>(std::numeric_limits<int>::max())) {
            std::cerr << "command too long to send" << std::endl;
            return;
        }

        send(sock, cmd.c_str(), static_cast<int>(cmd.length()), 0);
    };
};


#endif //FTP_CLIENT_FTPCLIENT_H
