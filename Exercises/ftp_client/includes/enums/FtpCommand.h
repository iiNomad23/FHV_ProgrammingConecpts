//
// Created by marco on 27/02/2024.
//

#ifndef FTP_CLIENT_FTPCOMMAND_H
#define FTP_CLIENT_FTPCOMMAND_H

#include <string>

enum FtpCommand {
    LS,
    GET,
    ASCII,
    BINARY,
    EXIT,
    UNKNOWN
};

FtpCommand stringToFtpCommand(const std::string &commandStr) {
    if (commandStr == "ls") {
        return LS;
    }
    if (commandStr == "get") {
        return GET;
    }
    if (commandStr == "ascii") {
        return ASCII;
    }
    if (commandStr == "binary") {
        return BINARY;
    }
    if (commandStr == "exit") {
        return EXIT;
    }

    return UNKNOWN;
}

#endif //FTP_CLIENT_FTPCOMMAND_H
