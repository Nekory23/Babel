/*
** EPITECH PROJECT, 2021
** arcade
** File description:
** Error
*/

#include "Error.hpp"

Error::Error(std::ostream &os, const std::string &msg) throw() : _os(os)
{
    _msg = msg;
}

const char *Error::what() const throw()
{
    return _msg.c_str();
}