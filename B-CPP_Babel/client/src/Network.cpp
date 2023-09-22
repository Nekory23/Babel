/*
** EPITECH PROJECT, 2021
** babel
** File description:
** Network
*/

#include "Network.hpp"

Nt::udp_client::udp_client(asio::io_context &io, char *ip, char *port) : _socket(io, asio::ip::udp::endpoint(asio::ip::udp::v4(), std::atoi(port))), _resolver(_io_context)
{
    _res = _resolver.resolve(asio::ip::udp::v4(), ip, port);
    Receive();
}

void Nt::udp_client::Receive()
{
    _socket.async_receive_from(asio::buffer(_data, _data.size()), _sender, 
    [this](std::error_code ec, std::size_t bytes_recvd) {
          if (!ec && bytes_recvd > 0)
            Send(bytes_recvd);
          else
            Receive();
    });
}

void Nt::udp_client::Send(std::size_t size)
{
    _socket.async_send_to(
    asio::buffer(_data, _data.size()), _sender,
    [this](std::error_code, std::size_t) {
      Receive();
    });
}

void Nt::udp_client::Run()
{
    _io_context.run();
}

void Nt::udp_client::SetData(std::vector<unsigned char> data)
{
    _data = data;
}

std::vector<unsigned char> Nt::udp_client::GetData()
{
    return _data;
}