//
// Created by marco on 25/02/2024.
//

#include "../includes/FtpSocket.h"

FtpSocket::FtpSocket() = default;

FtpSocket::~FtpSocket() {
    WSACleanup();
}