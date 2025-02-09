#include "Wave.hpp"
#include <memory>
#include <cmath>

Wave::Wave(const std::string &path) 
{
    file.open(path);
    if (!file.good())
    {
        throw std::runtime_error("Unable to open \"" + path + "\", file does not exist");
    }

    // Validate the file
    try {
        file.read(reinterpret_cast<char*>(&header), sizeof(Header));
        if (std::string(reinterpret_cast<char*>(header.riff), 4) != "RIFF" || std::string(reinterpret_cast<char*>(header.fileType), 4) != "WAVE")
            throw std::runtime_error("Error when opening \"" + path + "\", file is not a WAVE");

        if (std::string(reinterpret_cast<char*>(header.formatMarker), 4) != "fmt ")
            throw std::runtime_error("Error when opening \"" + path + "\", file is corrupted");

        if (header.format != 1 ||
            (header.numOfChannels != 1 && header.numOfChannels != 2) ||
            (header.bitsPerSample != 8 && header.bitsPerSample != 16))
            throw std::runtime_error("Error when opening \"" + path + "\", unsupported WAVE format");

        if (std::string(reinterpret_cast<char*>(header.dataMarker), 4) != "data")
            throw std::runtime_error("Error when opening \"" + path + "\", file is corrupted");
    }

    catch (...) 
    {
        file.close();
        throw;
    }
}

Wave::~Wave() 
{
    file.close();
}

std::size_t Wave::getSampleRate() const 
{
    return header.sampleRate;
}

bool Wave::readSamples(std::size_t num, std::vector <float> &samples) 
{
    unsigned int readSize = num * header.bytesPerSample;
    unsigned int leftSize = header.dataSize - offset;

    // If we are running out of data to read, cap the amount
    if (readSize > leftSize) {
        readSize = leftSize - (leftSize % header.bytesPerSample);
        num = readSize / header.bytesPerSample;
    }

    // Read the raw data
    auto data = std::make_unique<std::uint8_t[]>(readSize);
    file.read(reinterpret_cast<char*>(data.get()), readSize);

    // Mono
    if (header.numOfChannels == 1) {
        if (header.bitsPerSample == 8) {
            for (std::size_t i = 0; i < num; i++)
                samples.push_back(static_cast<float>(data[i]) / 255.0f);
        }

        else if (header.bitsPerSample == 16) {
            for (std::size_t i = 0; i < num; i++)
            {
                std::int16_t sample = data[i*2+0] | (data[i*2+1] << 8);
                samples.push_back(static_cast<float>(sample) / 65535.0f);
            }
        }
    }

    // Stereo
    else if (header.numOfChannels == 2) {
        if (header.bitsPerSample == 8) {
            for (std::size_t i = 0; i < num; i++) {
                std::int16_t sample = (data[i*2+0] + data[i*2+1]) / 2;
                samples.push_back(static_cast<float>(sample) / 255.0f);
            }
        }

        else if (header.bitsPerSample == 16) {
            for (std::size_t i = 0; i < num; i++) {
                std::uint32_t sample = ((data[i*4+0] | (data[i*4+1] << 8)) + (data[i*4+2] | (data[i*4+3] << 8))) / 2;
                samples.push_back(static_cast<float>(sample) / 65535.0f);
            }
        }
    }

    return leftSize - readSize == 0;
}
