#pragma once

#include "Clock.hpp"

// Pulse-Width-Modulator
class Pwm : public Clock
{
public:
    Pwm(std::uint32_t sampleRate);
    ~Pwm();

    inline volatile std::uint32_t &getFifoInput() {
        return regs->fifoInput;
    }

private:
    struct Regs {
        std::uint32_t control;   // Control
        std::uint32_t status;    // Status
        std::uint32_t dmaConfig; // DMA Configuration
        std::uint32_t reserved1; // Reserved 1
        std::uint32_t ch1Range;  // Channel 1 Range
        std::uint32_t ch1Data;   // Channel 2 Data
        std::uint32_t fifoInput; // FIFO Input
        std::uint32_t reserved2; // Reserved 2
        std::uint32_t ch2Range;  // Channel 2 Range
        std::uint32_t ch2Data;   // Channel 2 Data
    };

    volatile Regs *regs;
};
