//
// Created by marco on 27/02/2024.
//

#ifndef FTP_CLIENT_RESPONSEFAILUREEXCEPTION_H
#define FTP_CLIENT_RESPONSEFAILUREEXCEPTION_H

#include <stdexcept>

class ResponseFailureException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

#endif //FTP_CLIENT_RESPONSEFAILUREEXCEPTION_H
