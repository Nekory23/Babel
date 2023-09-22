/*
** EPITECH PROJECT, 2021
** EpitechTools
** File description:
** BabelServer
*/

#ifndef MAIN_HPP_
#define MAIN_HPP_
#define ASIO_STANDALONE

#include <iostream>
#include <asio.hpp>
#include <asio/ts/internet.hpp>
#include "ServError.hpp"

class Server_Connection {
    public:
        Server_Connection(int port);
        ~Server_Connection() {};
        void connection();
        void close();
    private:
    asio::error_code ec;
    asio::ip::tcp::endpoint ToConnectTo;
    asio::io_context context;
    asio::ip::tcp::socket socket;
};


/* STATIC CONST */
static const int SUCCESS = 0;
static const int FAIL = 1;
static const int ERROR = 84;

#endif /* !MAIN_HPP_ */
