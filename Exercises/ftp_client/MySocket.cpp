//
// Created by marco on 25/02/2024.
//

#include "MySocket.h"

MySocket::MySocket() = default;

MySocket::~MySocket() {
    WSACleanup();
}