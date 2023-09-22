/*
** EPITECH PROJECT, 2021
** EpitechTools
** File description:
** AsioConnection
*/

#include "BabelServer.hpp"

Server_Connection::Server_Connection(int port): ToConnectTo(asio::ip::make_address("0.0.0.0", ec), port), context(), socket(context) {}

void Server_Connection::connection() 
{
    this->socket.connect(this->ToConnectTo, this->ec);
    if (ec)
        std::cerr << "It seems like the connection failed" << std::endl;
    else
        std::cout << "Connected." << std::endl;
}

void Server_Connection::close() 
{
    this->socket.close();   
}
