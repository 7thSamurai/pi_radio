#pragma once

#include "Clock.hpp"

class Output : public Clock
{
public:
    Output(std::uint32_t divisor);
    ~Output();

    inline void set_divisor(std::uint32_t divisor) 
    {
        regs->divisors = (0x5a << 24) | (divisor & 0xffffff);
    }

    inline volatile std::uint32_t &getDivisor() 
    {
        return regs->divisors;
    }

private:
    volatile std::uint32_t *output;
};
