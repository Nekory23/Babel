/*
** EPITECH PROJECT, 2021
** B-CPP_Babel
** File description:
** test
*/

#ifndef _TEST_HPP_
#define _TEST_HPP_

typedef float SAMPLE;

static const int NUM_SECONDS = 5;
static const int SAMPLE_RATE = 44100;
static const int NUM_CHANNELS = 2;
static const int FRAMES_PER_BUFFER = 512;
static const float SAMPLE_SILENCE = 0.0f;

typedef struct paTestData
{
    int frameIndex;
    int maxFrameIndex;
    SAMPLE *recordedSamples;
} paTestData;

#endif /*_TEST_HPP_*/