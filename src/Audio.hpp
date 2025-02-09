#pragma once

#include <string>
#include <vector>
#include <fstream>

class Audio
{
public:
    virtual ~Audio() = default;

    virtual std::size_t getSampleRate() const = 0;

    // Returns true if out of samples
    virtual bool readSamples(std::size_t num, std::vector <float> &samples) = 0;
};
