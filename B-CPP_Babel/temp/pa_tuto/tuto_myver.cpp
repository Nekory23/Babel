#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "portaudio.h"
#include "test.hpp"

static int finish_prog(PaError error, paTestData *data)
{
    Pa_Terminate();
    if (data->recordedSamples)
        free(data->recordedSamples);
    if (error != paNoError) {
        fprintf( stderr, "An error occurred while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", error);
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText(error));
        error = 1;
    }
    return error;
}

static void fill_data(paTestData *data, PaError error, int numBytes, int numSamples)
{
    data->maxFrameIndex = NUM_SECONDS * SAMPLE_RATE;
    data->frameIndex = 0;
    data->recordedSamples = (SAMPLE *) malloc(numBytes);
    if (data->recordedSamples == NULL) {
        std::cerr << "cannot allocate record array" << std::endl;
        finish_prog(error, data);
    }
    for (int i = 0; i < numSamples; ++i)
        data->recordedSamples[i] = 0;
    error = Pa_Initialize();
    if (error != paNoError)
        finish_prog(error, data);
}

static void fill_input(PaStreamParameters *input, paTestData *data, PaError error)
{
    input->device = Pa_GetDefaultInputDevice();
    if (input->device == paNoDevice) {
        std::cerr << "no default input device" << std::endl;
        finish_prog(error, data);
    }
    input->channelCount = 2;
    input->sampleFormat = paFloat32;
    input->suggestedLatency = Pa_GetDeviceInfo(input->device)->defaultLowInputLatency;
    input->hostApiSpecificStreamInfo = NULL;
}

static void fill_output(PaStreamParameters *output, paTestData *data, PaError error)
{
    output->device = Pa_GetDefaultOutputDevice();
    if (output->device == paNoDevice) {
        std::cerr << "no default output device" << std::endl;
        finish_prog(error, data);
    }
    output->channelCount = 2;
    output->sampleFormat = paFloat32;
    output->suggestedLatency = Pa_GetDeviceInfo(output->device)->defaultLowOutputLatency;
    output->hostApiSpecificStreamInfo = NULL;
}

static int recordCallBack(const void *input, void *output, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlag, void *user)
{
    paTestData *data = (paTestData*)user;
    const SAMPLE *rptr = (const SAMPLE*)input;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    //long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) output; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlag;
    (void) user;

    if( framesLeft < framesPerBuffer ) {
        framesToCalc = framesLeft;
        finished = paComplete;
    } else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( input == NULL ) {
        for(long i=0; i<framesToCalc; i++ ) {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    } else {
        for(long i=0; i<framesToCalc; i++ ) {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return (finished);
}

static int playCallBack(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        /* final buffer... */
        for( i=0; i<framesLeft; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        for( ; i<framesPerBuffer; i++ )
        {
            *wptr++ = 0;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

int main(void)
{
    // CONSTRUCT
    PaStreamParameters input;
    PaStreamParameters output;
    PaStream *stream;
    PaError error = paNoError;
    paTestData data;
    int totalFrames = NUM_SECONDS * SAMPLE_RATE;
    int numSamples = totalFrames * NUM_CHANNELS;
    int numBytes = numSamples * sizeof(SAMPLE);

    std::cout << "test record\n" << std::endl;

    fill_data(&data, error, numBytes, numSamples);
    fill_input(&input, &data, error);
    fill_output(&output, &data, error);

    // END
    // fonction qui detecte lorsque tu parle qui call start_talk //

    // fonction -> une fois le call en is ok | name = start_talk
    error = Pa_OpenStream(&stream, &input, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, recordCallBack, &data);
    if (error != paNoError) {
        std::cerr << "cannot open stream" << std::endl;
        finish_prog(error, &data);
    }
    error = Pa_StartStream(stream);
    if (error != paNoError) {
        std::cerr << "cannot start stream" << std::endl;
        finish_prog(error, &data);
    }

    std::cout << std::endl << "=== Now recording!! Please speak into the microphone. ===" << std::endl;
    while ((error = Pa_IsStreamActive(stream)) == 1) {
        Pa_Sleep(1000);
        std::cout << "index = " << data.frameIndex << std::endl;
    }
    if (error < 0) {
        std::cerr << "ERROR" << std::endl;
        finish_prog(error, &data);
    }
    error = Pa_CloseStream(stream);
    if (error < 0) {
        std::cerr << "cannot close stream" << std::endl;
        finish_prog(error, &data);
    }

    data.frameIndex = 0;
    // END
    std::cout << std::endl << "=== Now playing back. ===" << std::endl;
    // SPEAK
    error = Pa_OpenStream(&stream, NULL, &output, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, playCallBack, &data);
    if (error != paNoError) {
        std::cerr << "cannot open stream" << std::endl;
        finish_prog(error, &data);
    }

    if (stream) {
        error = Pa_StartStream(stream);
        if (error != paNoError) {
            std::cerr << "cannot open stream" << std::endl;
            finish_prog(error, &data);
        }
        while ((error = Pa_IsStreamActive(stream)) == 1)
            Pa_Sleep(1000);
        if (error < 0) {
            std::cerr << "ERROR 2" << std::endl;
            finish_prog(error, &data);
        }
        error = Pa_CloseStream(stream);
        if (error != paNoError) {
            std::cerr << "cannot open stream" << std::endl;
            finish_prog(error, &data);
        }
        std::cout << "done." << std::endl;
    }
    // END
    return finish_prog(error, &data); // DESTRUCT
}