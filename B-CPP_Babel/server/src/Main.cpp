/*
** EPITECH PROJECT, 2021
** EpitechTools
** File description:
** Main
*/

#include "BabelServer.hpp"
#include "ServError.hpp"

int main (int ac, char **av)
{
    int ret = SUCCESS;
    
    if (ac != 2) {
        std::cerr << "Error: pls add a PORT" << std::endl;
        return ERROR;
    }
    try {
    }
    catch (conextionFail &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        ret = 84;
    }
    return ret;
}
