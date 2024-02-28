//
// Created by marco on 25/02/2024.
//

#ifndef FTP_CLIENT_FTPCLIENT_H
#define FTP_CLIENT_FTPCLIENT_H

#include <string>
#include <iostream>
#include <limits>
#include <regex>
#include "FtpSocket.h"
#include "exeptions/ParsePasvFailureException.h"

class FtpClient {
private:
    FtpSocket _controlSocket;
    std::string _ftpServerIp;

public:
    FtpClient(const std::string &serverIP, uint16_t port);

    ~FtpClient();

    void login(const std::string &userName, const std::string &password) const;

    void close();

    void ls();

    void get(const std::string& fileName);

    void setAsciiMode();

    void setBinaryMode();

    size_t getFileSize(const std::string& fileName);

    static void displayProgress(size_t received, size_t total);

    static int16_t parseResponseCode(const std::string &response) {
        try {
            return static_cast<int16_t>(std::stoi(response.substr(0, 3)));
        } catch (const std::exception &e) {
            std::cerr << "Error parsing response code: " << e.what() << std::endl;
            return 0;
        }
    }

    static uint16_t parsePasvResponse(const std::string &response, uint16_t &port) {
        std::regex pasvPattern(R"(\((\d+),(\d+),(\d+),(\d+),(\d+),(\d+)\))");
        std::smatch matches;

        if (std::regex_search(response, matches, pasvPattern)) {
            if (matches.size() == 7) {
                int p1 = std::stoi(matches[5].str());
                int p2 = std::stoi(matches[6].str());
                return (p1 * 256) + p2;
            } else {
                throw ParsePasvFailureException("Failed to parse PASV response: " + response);
            }
        } else {
            throw ParsePasvFailureException("PASV response does not match expected format: " + response);
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
        std::cout << "exit: closes the connection and the process" << std::endl;
        std::cout << std::endl;
    }
};


#endif //FTP_CLIENT_FTPCLIENT_H
