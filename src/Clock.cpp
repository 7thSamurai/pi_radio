#include "Clock.hpp"

Clock::Clock(std::uintptr_t addr, std::uint32_t divisor) 
{
    auto &peripherals = Peripherals::getInstance();
    regs = reinterpret_cast<Regs*>(peripherals.getVirtualAddress(addr));
    
    regs->control = (0x5a << 24) | 6;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));

    regs->divisors = (0x5a << 24) | (divisor & 0xffffff);
    regs->control  = (0x5a << 24) | (1 << 9) | (1 << 4) | 6;
}

Clock::~Clock() 
{
    regs->control = (0x5a << 24) | 6;
}
