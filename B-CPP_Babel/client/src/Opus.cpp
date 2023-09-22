/*
** EPITECH PROJECT, 2021
** B-CPP_Babel
** File description:
** Opus
*/

#include "Error.hpp"
#include "LibAudio.hpp"

LibAudio::Opus::Opus()
{
    _opusError = OPUS_OK;
    _enc_bytes = 0;
    _dec_bytes = 0;
    _framesProcessed = 0;
    for (int i = 0; i != BUFFER_SIZE * NUM_CHANNELS; ++i) {
        _captured.push_back(0);
        _decoded.push_back(0);
    }
    for (int i = 0; i != BUFFER_SIZE * NUM_CHANNELS * 2; ++i)
        _encoded.push_back(0);
    _encoder = opus_encoder_create(SAMPLE_RATE, NUM_CHANNELS, OPUS_APPLICATION_AUDIO, &_opusError);
    if (_opusError != OPUS_OK)
        throw Error(std::cerr, "Opus: opus_encoder_create failed");
    std::cout << "opus_encoder :OK" << std::endl;
    _decoder = opus_decoder_create(SAMPLE_RATE, NUM_CHANNELS, &_opusError);
    if (_opusError != OPUS_OK)
        throw Error(std::cerr, "Opus: opus_decoder_create failed");
    std::cout << "opus_decoder :OK" << std::endl;
}

LibAudio::Opus::~Opus()
{
    opus_decoder_destroy(_decoder);
    opus_encoder_destroy(_encoder);
}

void LibAudio::Opus::Encode()
{
    _enc_bytes = opus_encode(_encoder, reinterpret_cast<opus_int16 const*>(_captured.data()), 480, _encoded.data(), _encoded.size());
    if (_enc_bytes < 0) {
        std::cout << "opus_encode failed: " << _enc_bytes << "\n";
        throw Error(std::cerr, "Opus: opus_encode failed");
    }
    std::cout << "opus_encode :OK" << std::endl;
}

void LibAudio::Opus::Decode()
{
    _dec_bytes = opus_decode(_decoder, _encoded.data(), _enc_bytes, reinterpret_cast<opus_int16*>(_decoded.data()), BUFFER_SIZE, 0);
    if (_dec_bytes < 0)
        throw Error(std::cerr, "Opus: opus_decode failed");
    std::cout << "opus_decode :OK" << std::endl;
}

void LibAudio::Opus::SetCaptured(std::vector<float> newCapture) noexcept
{
    _captured = newCapture;
}

std::vector<float> LibAudio::Opus::GetCaptured() const noexcept
{
    return _captured;
}

std::vector<unsigned short> LibAudio::Opus::GetDecoded() const noexcept
{
    return _decoded;
}

std::vector<unsigned char> LibAudio::Opus::GetEncoded() const noexcept
{
    return _encoded;
}

void LibAudio::Opus::SetEncoded(std::vector<unsigned char> data) noexcept
{
    _encoded = data;
}