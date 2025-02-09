#pragma once

#include "Device.hpp"

class Clock : public Device
{
public:
    Clock(std::uintptr_t addr, std::uint32_t divisor);
    ~Clock();

protected:
    struct Regs {
        std::uint32_t control;
        std::uint32_t divisors;
    };

    volatile Regs *regs;
};
