/*
** EPITECH PROJECT, 2021
** babel
** File description:
** Network
*/

#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <vector>
#include "asio.hpp"

namespace Nt {
    class udp_client {
        public:
            udp_client(asio::io_context &io, char *ip, char *port);
            ~udp_client() = default;

            void Receive();
            void ReceiveFunc(std::error_code ec, std::size_t bytes_recvd);
            void Send(std::size_t length);
            void Run();

            void SetData(std::vector<unsigned char> data);
            std::vector<unsigned char> GetData();

        protected:
        private:
            asio::ip::udp::socket _socket;
            asio::ip::udp::endpoint _sender;
            asio::io_context _io_context;
            asio::ip::udp::resolver _resolver;
            asio::ip::udp::resolver::results_type _res;

            std::vector<unsigned char> _data;
    };
}

#endif /* !NETWORK_HPP_ */
