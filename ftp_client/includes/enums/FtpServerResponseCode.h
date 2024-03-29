//
// Created by marco on 27/02/2024.
//

#ifndef FTP_CLIENT_FTPSERVERRESPONSECODE_H
#define FTP_CLIENT_FTPSERVERRESPONSECODE_H

enum FtpServerResponseCode : int16_t {
    FILE_STATUS_OKAY = 150,
    COMMAND_OKAY = 200,
    FILE_STATUS = 213,
    SERVICE_READY = 220,
    CLOSING_DATA_CONNECTION = 226,
    ENTERING_PASSIVE_MODE = 227,
    USER_LOGGED_IN = 230,
    USER_NAME_OKAY_NEED_PASSWORD = 331,
    FILE_NOT_FOUND = 550,
};

#endif //FTP_CLIENT_FTPSERVERRESPONSECODE_H
