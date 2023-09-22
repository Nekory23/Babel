/*
** EPITECH PROJECT, 2021
** EpitechTools
** File description:
** Main
*/

#include "Babel.hpp"
#include "Error.hpp"
#include "LibAudio.hpp"
#include "Network.hpp"

int main(int ac, char **av)
{
    int ret = SUCCESS;

    if (ac != 4) {
        std::cout << "Error: invalid number of arguments (./babel_client IP PORT MODE)" << std::endl;
    }
    try {
        LibAudio::PortAudio audio;
        LibAudio::Opus opus;
        asio::io_context io_context;
        Nt::udp_client client(io_context, av[1], av[2]);

        client.Run();
        if (!std::strcmp(av[3], "1")) {
            audio.recordVoice();
            opus.SetCaptured(audio.GetRecordedSamples());
            opus.Encode();
            client.SetData(opus.GetEncoded());
            client.Send(opus.GetEncoded().size());
        }
        else {
            client.Receive();
            opus.SetEncoded(client.GetData());
            opus.Decode();
            audio.SetRecordedSamples(opus.GetCaptured());
            audio.playRecorded();
        }
    }
    catch (Error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        ret = ERROR;
    }
    catch (...) {}
    return SUCCESS;
}
