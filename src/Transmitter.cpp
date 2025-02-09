#include "Transmitter.hpp"
#include "Dma.hpp"
#include "Memory.hpp"
#include "Pwm.hpp"
#include "Output.hpp"

#include <cmath>
#include <iostream>

#define BUFFER_TIME 1000000
#define PWM_WRITES_PER_SAMPLE 10

bool Transmitter::transmitting = false;

Transmitter::Transmitter()
    : stopped(true)
{
}

Transmitter::~Transmitter() 
{
}

void Transmitter::transmit(Audio &audio, float frequency, float bandwidth) 
{
    if (transmitting)
        throw std::runtime_error("Cannot transmit, transmitter already in use");

    transmitting = true;
    stopped = false;

    try {
        transmitDma(audio, frequency, bandwidth, 0);
    }

    catch (...) {
        transmitting = false;
        throw;
    }

    transmitting = false;
}

void Transmitter::stop() 
{
    stopped = true;
}

void Transmitter::transmitDma(Audio &audio, float frequency, float bandwidth, unsigned int dmaChannel) 
{
    if (dmaChannel > 15)
        throw std::runtime_error("Invalid DMA channel " + std::to_string(dmaChannel) + ", legal-value is 0-15");

    unsigned int bufferSize   = static_cast<unsigned long long>(audio.getSampleRate()) * BUFFER_TIME / 1000000; // FIXME??
    unsigned int clockDivisor = static_cast<unsigned int>(std::round(Peripherals::getClockFrequency() * (1 << 12) / frequency));
    unsigned int divisorRange = clockDivisor - static_cast<unsigned int>(std::round(Peripherals::getClockFrequency() * (1 << 12) / (frequency + bandwidth * 0.0005f)));

    std::vector <float> samples;
    audio.readSamples(bufferSize, samples);

    if (!output)
        output = std::make_unique<Output>(clockDivisor);

    Memory memory(
        (2 * bufferSize * sizeof(Dma::ControlBlock)) + // dma_cb
        (bufferSize * sizeof(std::uint32_t)) +         // clock_div
        sizeof(std::uint32_t)                          // pwm_fifo_data
    );

    Pwm pwm(audio.getSampleRate());
    auto &peripherals = Peripherals::getInstance();

    volatile auto *dmaCb = reinterpret_cast<Dma::ControlBlock*>(memory.getBaseAddress());
    volatile auto *clockDiv = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(dmaCb) + 2 * bufferSize * sizeof(Dma::ControlBlock)
    );
    volatile auto *pwmFifoData = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(clockDiv) + bufferSize * sizeof(std::uint32_t)
    );

    // Setup the DMA control blocks
    std::size_t offset = 0;
    for (std::size_t i = 0; i < bufferSize; i++) {
        clockDiv[i] = (0x5a << 24) | ((clockDivisor - static_cast<std::int32_t>(std::round(samples[i] * divisorRange))) & 0xffffff);

        // Update the Sample
        dmaCb[offset].transferInfo = (1 << 26) | (1 << 3);
        dmaCb[offset].srcAddr      = memory.getPhysicalAddress(&clockDiv[i]);
        dmaCb[offset].destAddr     = peripherals.getPhysicalAddress(&output->getDivisor());
        dmaCb[offset].transferLen  = sizeof(std::uint32_t);
        dmaCb[offset].stride       = 0;
        dmaCb[offset].nextCb      = memory.getPhysicalAddress(&dmaCb[offset + 1]);
        offset++;

        // Delay until time for the next Sample
        dmaCb[offset].transferInfo = (1 << 26) | (0x05 << 16) | (1 << 6) | (1 << 3);
        dmaCb[offset].srcAddr      = memory.getPhysicalAddress(pwmFifoData);
        dmaCb[offset].destAddr     = peripherals.getPhysicalAddress(&pwm.getFifoInput());
        dmaCb[offset].transferLen  = PWM_WRITES_PER_SAMPLE * sizeof(std::uint32_t);
        dmaCb[offset].stride       = 0;
        dmaCb[offset].nextCb       = memory.getPhysicalAddress((i < bufferSize-1) ? &dmaCb[offset + 1] : dmaCb);
        offset++;
    }

    *pwmFifoData = 0x00000000;
    Dma dma(memory.getPhysicalAddress(dmaCb), dmaChannel);
    std::this_thread::sleep_for(std::chrono::microseconds(BUFFER_TIME / 4));

    // Clean up routine
    auto finally = [&]() {
        dmaCb[(offset < 2 * bufferSize) ? offset : 0].nextCb = 0x00000000;

        while (dma.getcontrolBlockAddress())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }

        stopped = true;
    };

    try {
        while (!stopped) {
            // Refill the samples
            samples.clear();
            audio.readSamples(bufferSize, samples);
            if (samples.empty())
                break;

            offset = 0;
            for (std::size_t i = 0; i < samples.size(); i++) {
                // Wait until ready to add new samples
                while (i == ((dma.getcontrolBlockAddress() - memory.getPhysicalAddress(dmaCb)) / (2 * sizeof(Dma::ControlBlock)))) {
                    std::this_thread::sleep_for(std::chrono::microseconds(1000));
                }

                clockDiv[i] = (0x5a << 24) | ((clockDivisor - static_cast<std::int32_t>(std::round(samples[i] * divisorRange))) & 0xffffff);
                offset += 2;
            }
        }
    }

    catch (...) {
        finally();
        throw;
    }

    finally();
}
