#include <iostream>
#include <algorithm>
#include <memory>
#include <filesystem>
#include <csignal>

#include "Transmitter.hpp"
#include "Wave.hpp"

std::unique_ptr <Transmitter> transmitter;
bool stop = false;

// Interrupt Signal handler
void sigIntHandler(int sig) {
    // Make sure to stop the transmitter
    if (transmitter) {
        std::cout << "Stopping..." << std::endl;
        transmitter->stop();
        stop = true;
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << argv[0] << " [FREQUENCY] [FILES...]" << std::endl;
        return 1;
    }

    float frequency = std::atof(argv[1]);
    float bandwith  = 200.0f; // TODO: Find a good value for this

    signal(SIGINT, sigIntHandler);

    try {
        std::cout << "Broadcasting at " << frequency << " MHz" << std::endl;
        transmitter = std::make_unique<Transmitter>();

        // Grab the input files and validate them
        std::vector<std::filesystem::path> files;
        for (int i = 2; i < argc; i++) {
            files.push_back(std::filesystem::path(argv[i]));

            // Make sure that it exists
            if (!std::filesystem::exists(files.back()))
                throw std::runtime_error("File \"" + files.back().string() + "\" does not exist");

            // Make sure that it's in a supported format
            else if (files.back().extension() != ".wav")
                throw std::runtime_error("File \"" + files.back().string() + "\" is in an unsupported format");
        }

        // Loop through each file and broadcast it
        for (auto &file : files) {
            std::cout << "Playing \"" << file.string() << "\"" << std::endl;

            // Load the file based on it's extension
            std::unique_ptr<Audio> audio;
            if (file.extension() == ".wav")
                audio = std::make_unique<Wave>(file);

            // Transmit!
            transmitter->transmit(*audio, frequency, bandwith);
        }
    }

    // Catch any errors
    catch (std::exception &catched) {
        std::cout << "[Error] " << catched.what() << std::endl;
        return 1;
    }

    return 0;
}
