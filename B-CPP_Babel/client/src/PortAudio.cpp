/*
** EPITECH PROJECT, NUM_CHANNELS0NUM_CHANNELS1
** B-CPP_Babel
** File description:
** LibAudio::PortAudio
*/

#include <iomanip>
#include <stdlib.h>
#include "LibAudio.hpp"
#include "Error.hpp"

static void fillInputParam(PaStreamParameters *input)
{
    input->device = Pa_GetDefaultInputDevice();
    if (input->device == paNoDevice)
        throw Error(std::cerr, "PortAudio: No input device");
    input->channelCount = NUM_CHANNELS;
    input->sampleFormat = paFloat32;
    input->suggestedLatency = Pa_GetDeviceInfo(input->device)->defaultLowInputLatency;
    input->hostApiSpecificStreamInfo = NULL;
}

static void fillOutputParam(PaStreamParameters *output)
{
    output->device = Pa_GetDefaultOutputDevice();
    if (output->device == paNoDevice)
        throw Error(std::cerr, "PortAudio : No input device");
    output->channelCount = NUM_CHANNELS;
    output->sampleFormat = paFloat32;
    output->suggestedLatency = Pa_GetDeviceInfo(output->device)->defaultLowOutputLatency;
    output->hostApiSpecificStreamInfo = NULL;
}

LibAudio::PortAudio::PortAudio() :  _sampleSilence(0.0f)
{
    int totalFrames = DURATION * SAMPLE_RATE;
    int numSamples = totalFrames * NUM_CHANNELS;
    int numBytes = numSamples * sizeof(float);

    _error = paNoError;
    _data.maxFrameIndex = DURATION * SAMPLE_RATE;
    _data.frameIndex = 0;
    _data.recordedSamples = reinterpret_cast<float *>(malloc(sizeof(float) * numBytes));
    //for (int i = 0; i != BUFFER_SIZE * NUM_CHANNELS; ++i)
    //    _recordedSamples.push_back(0);
    for (int i = 0; i < (DURATION * SAMPLE_RATE * NUM_CHANNELS); ++i)
        _data.recordedSamples[i] = 0;
    _error = Pa_Initialize();
    if (_error != paNoError)
        throw Error(std::cerr, "PortAudio : Cannot initialize of LibAudio::PortAudio\n");
    fillInputParam(&_inputParam);
    fillOutputParam(&_outputParam);
}

LibAudio::PortAudio::~PortAudio()
{
    Pa_Terminate();
}

static int record(const void *input, void *, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *user)
{
    paData_t *data = reinterpret_cast<paData_t *>(user);
    const float *rptr = reinterpret_cast<const float *>(input);
    float *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    if (framesLeft < framesPerBuffer) {
        framesToCalc = framesLeft;
        finished = paComplete;
    } else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }
    if (input == NULL) {
        for(long i = 0; i < framesToCalc; ++i) {
            *wptr++ = 0.0f;
            if(NUM_CHANNELS == 2)
                *wptr++ = 0.0f;
        }
    } else {
        for(long i = 0; i < framesToCalc; ++i) {
            *wptr++ = *rptr++;
            if(NUM_CHANNELS == 2)
                *wptr++ = *rptr++;
        }
    }
    data->frameIndex += framesToCalc;
    return (finished);
}

void LibAudio::PortAudio::recordVoice()
{
    _error = Pa_OpenStream(&_stream, &_inputParam, NULL, SAMPLE_RATE, BUFFER_SIZE, paClipOff, record, &_data);
    if (_error != paNoError)
        throw Error(std::cerr, "PortAudio: Cannot open stream");
    _error = Pa_StartStream(_stream);
    if (_error != paNoError)
        throw Error(std::cerr, "PortAudio: Cannot start stream");
    while ((_error = Pa_IsStreamActive(_stream)) == 1)
        Pa_Sleep(1000);
    if (_error < 0)
        throw Error(std::cerr, "PortAudio: Error stream");
    _error = Pa_CloseStream(_stream);
    if (_error < 0)
        throw Error(std::cerr, "PortAudio: Cannot close stream");
}

static int play(const void *, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *userData)
{
    paData_t *data = reinterpret_cast<paData_t *>(userData);
    float *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    float *wptr = reinterpret_cast<float *>(outputBuffer);
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    if (framesLeft < framesPerBuffer) {
        for (i = 0; i < framesLeft; ++i) {
            *wptr++ = *rptr++;
            if (NUM_CHANNELS == 2)
                *wptr++ = *rptr++;
        }
        for (; i < framesPerBuffer; ++i) {
            *wptr++ = 0;
            if (NUM_CHANNELS == 2)
                *wptr++ = 0;
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    } else {
        for (i = 0; i < framesPerBuffer; ++i) {
            *wptr++ = *rptr++;
            if (NUM_CHANNELS == 2)
                *wptr++ = *rptr++;
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

void LibAudio::PortAudio::playRecorded()
{
    _data.frameIndex = 0;
    _error = Pa_OpenStream(&_stream, NULL, &_outputParam, SAMPLE_RATE, BUFFER_SIZE, paClipOff, play, &_data);
    if (_error != paNoError)
        throw Error(std::cerr, "PortAudio: Cannot open stream");
    if (_stream) {
        _error = Pa_StartStream(_stream);
        if (_error != paNoError)
            throw Error(std::cerr, "PortAudio: Cannot start stream");
        while ((_error = Pa_IsStreamActive(_stream)) == 1)
            Pa_Sleep(1000);
        if (_error < 0)
            throw Error(std::cerr, "PortAudio: Error stream");
        _error = Pa_CloseStream(_stream);
        if (_error != paNoError)
            throw Error(std::cerr, "PortAudio: Cannot close stream");
    }
}

std::vector<float> LibAudio::PortAudio::GetRecordedSamples() noexcept
{
    float temp = 0;

    for (int i = 0; i != BUFFER_SIZE; ++i) {
        temp = _data.recordedSamples[i];
        _recordedSamples.push_back(temp);
        //std::cout << "Portaudio : " << std::fixed << std::setprecision(10) << _data.recordedSamples[i] << "    " << temp << std::endl;
    }
    return _recordedSamples;
}

void LibAudio::PortAudio::SetRecordedSamples(std::vector<float> recorded) noexcept
{
    float temp = 0;

    _recordedSamples = recorded;
    for (int i = 0; i != _recordedSamples.size(); ++i) {
        temp = _recordedSamples[i];
        _data.recordedSamples[i] = temp;
    }
}