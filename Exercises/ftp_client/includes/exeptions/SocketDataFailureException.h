//
// Created by marco on 28/02/2024.
//

#ifndef FTP_CLIENT_SOCKETDATAFAILUREEXCEPTION_H
#define FTP_CLIENT_SOCKETDATAFAILUREEXCEPTION_H

#include <stdexcept>

class SocketDataFailureException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

#endif //FTP_CLIENT_SOCKETDATAFAILUREEXCEPTION_H
