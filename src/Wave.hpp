#pragma once

#include "Audio.hpp"

class Wave : public Audio
{
public:
    Wave(const std::string &path);
    ~Wave();

    std::size_t getSampleRate() const override;

    // Returns true if out of samples
    bool readSamples(std::size_t num, std::vector <float> &samples) override;

private:
    struct Header {
        std::uint8_t riff[4]; // Should be "RIFF
        std::uint32_t fileSize;
        std::uint8_t fileType[4]; // Should be "WAVE"
        std::uint8_t formatMarker[4];
        std::uint32_t formatSize;
        std::uint16_t format;
        std::uint16_t numOfChannels;
        std::uint32_t sampleRate;
        std::uint32_t bytesPerSecond;
        std::uint16_t bytesPerSample;
        std::uint16_t bitsPerSample;
        std::uint8_t dataMarker[4];
        std::uint32_t dataSize;
    };

    std::ifstream file;
    
    Header header;
    unsigned int offset;
};
