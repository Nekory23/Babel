/*
** EPITECH PROJECT, 2021
** EpitechTools
** File description:
** ServError
*/
 
#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>
#include "BabelServer.hpp"

class conextionFail : public std::exception {
    public:

    virtual const char *what() const noexcept final {
        return "the connection have failed";
    } 
};

#endif /* "ERROR_HPP" */