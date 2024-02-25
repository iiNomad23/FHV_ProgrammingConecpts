cmake_minimum_required(VERSION 3.27)
project(ftp_client VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# Define the executable target
add_executable(ftp_client
        main.cpp
        MySocket.cpp
        MySocket.h
        FTPClient.cpp
        FTPClient.h
)

# Link static libraries to the executable target
# (In this case we need this for the lib 'libstdc++-6.dll' and some more)
# ----------------------------------------------------------------
# "PRIVATE" means that the libraries are only linked
# to the "ftp_client" target and not propagated to
# any other targets that link to "ftp_client".
# This is useful for keeping dependencies encapsulated within a target.
target_link_libraries(ftp_client
        PRIVATE -static
        PRIVATE ws2_32
)
