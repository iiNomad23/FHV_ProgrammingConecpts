//
// Created by marco on 25/02/2024.
//

#ifndef FTP_CLIENT_FTPCLIENT_H
#define FTP_CLIENT_FTPCLIENT_H

#include "FtpSocket.h"
#include <string>
#include <iostream>
#include <limits>

class FtpClient {
private:
    bool _connected;
    SOCKET _controlSocket;
    SOCKET _dataSocket;
    uint16_t _dataPort;
    std::string _dataIP;

public:
    FtpClient(const std::string& serverIP, uint16_t port);
    ~FtpClient();

    [[nodiscard]] bool isConnected() const;
    void login(const std::string& userName, const std::string& password) const;

    [[nodiscard]] int16_t close();
    [[nodiscard]] int16_t ls();
    [[nodiscard]] int16_t get();
    [[nodiscard]] int16_t ascii();
    [[nodiscard]] int16_t binary();

    static std::string receiveResponse(SOCKET sock) {
        char buffer[1024] = {};
        recv(sock, buffer, sizeof(buffer), 0);
        std::string response(buffer);
        std::cout << "Received: " << response << std::endl;
        return response;
    };

    static int16_t parseResponseCode(const std::string& response) {
        try {
            return static_cast<int16_t>(std::stoi(response.substr(0, 3)));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing response code: " << e.what() << " :(" << std::endl;
            return 0;
        }
    }

    static void displayCommands() {
        std::cout << std::endl;
        std::cout << "Command List:" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "ls: List directory contents" << std::endl;
        std::cout << "get <filename>: Download a file" << std::endl;
        std::cout << "ascii: Set transfer mode to ASCII" << std::endl;
        std::cout << "binary: Set transfer mode to binary" << std::endl;
        std::cout << "exit: closes the connection and exits the program" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    static void sendCommand(SOCKET sock, const std::string& cmd) {
        std::cout << "Sent: " << cmd;

        // ensure the command length does not exceed the maximum value for int
        if (cmd.length() > static_cast<std::string::size_type>(std::numeric_limits<int>::max())) {
            std::cerr << "Command too long to send" << " :(" << std::endl;
            return;
        }

        send(sock, cmd.c_str(), static_cast<int>(cmd.length()), 0);
    };
};


#endif //FTP_CLIENT_FTPCLIENT_H
