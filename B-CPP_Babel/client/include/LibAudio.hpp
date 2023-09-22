/*
** EPITECH PROJECT, 2021
** B-CPP_Babel
** File description:
** PortAudio
*/

#ifndef PORTAUDIO_HPP_
#define PORTAUDIO_HPP_

#include <vector>
#include "portaudio.h"
#include <opus/opus.h>

static const int NUM_CHANNELS = 2; // stereo audio
static const int BUFFER_SIZE = 480;
static const int SAMPLE_RATE = 48000;
static const int DURATION = 5;

struct paData_t {
    int frameIndex;
    int maxFrameIndex;
    float *recordedSamples;
};

namespace LibAudio {
    class PortAudio {
        public:
            PortAudio();
            ~PortAudio();

            void recordVoice(); // return a file to be compressed by Opus
            void playRecorded(); // play an audio file

            // getters
            std::vector<float> GetRecordedSamples() noexcept;
            // setters
            void SetRecordedSamples(std::vector<float> recorded) noexcept;

        protected:
        private:

            std::vector<float> _recordedSamples;

            float _sampleSilence;
            paData_t _data;
            PaError _error;
            PaStreamParameters _inputParam;
            PaStreamParameters _outputParam;
            PaStream *_stream;
    };

    class Opus {
        public:
            Opus();
            ~Opus();

            void Encode();
            void Decode();

            // getters
            std::vector<float> GetCaptured() const noexcept;
            std::vector<unsigned char> GetEncoded() const noexcept;
            std::vector<unsigned short> GetDecoded() const noexcept;
            // setters
            void SetCaptured(std::vector<float> newCapture) noexcept;
            void SetEncoded(std::vector<unsigned char> data) noexcept;

        protected:
        private:
            OpusEncoder *_encoder;
            OpusDecoder *_decoder;

            opus_int32 _enc_bytes;
            opus_int32 _dec_bytes;
            int _framesProcessed;
            int _opusError;
        
            std::vector<float> _captured;
            std::vector<unsigned short> _decoded;
            std::vector<unsigned char> _encoded;
    };
}

#endif /* !PORTAUDIO_HPP_ */