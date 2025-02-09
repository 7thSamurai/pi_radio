#pragma once

#include "Audio.hpp"
#include <memory>

class Output;

class Transmitter
{
public:
    Transmitter();
    ~Transmitter();

    void transmit(Audio &audio, float frequency, float bandwidth);
    void stop();

private:
    void transmitDma(Audio &audio, float frequency, float bandwidth, unsigned int dmaChannel);

    static bool transmitting;
    bool stopped;

    std::unique_ptr<Output> output;
};
