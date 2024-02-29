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
    explicit FtpClient(const std::string &serverIP, const FtpSocket &controlSocket);

    ~FtpClient();

    void login(const std::string &userName, const std::string &password) const;

    void close();

    void ls();

    void get(const std::string &fileName);

    void setAsciiMode();

    void setBinaryMode();

    size_t getFileSize(const std::string &fileName);

    static int16_t parseResponseCode(const std::string &response);

    static uint16_t parsePasvResponse(const std::string &response);

    static void displayCommands();
};


#endif //FTP_CLIENT_FTPCLIENT_H
