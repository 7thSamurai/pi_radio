#include "Pwm.hpp"

#define PWM_WRITES_PER_SAMPLE 10
#define PWM_CHANNEL_RANGE 32
#define PWMCLK_BASE_OFFSET 0x001010a0

Pwm::Pwm(std::uint32_t sampleRate) 
    : Clock(PWMCLK_BASE_OFFSET, Peripherals::getClockFrequency() * 1000000.0f * (1 << 12) / (PWM_WRITES_PER_SAMPLE * PWM_CHANNEL_RANGE * sampleRate)) 
{
    auto &peripherals = Peripherals::getInstance();
    regs = reinterpret_cast<Regs*>(peripherals.getVirtualAddress(PWM_BASE_OFFSET));
    regs->control = 0x00000000;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));

    // Reset
    regs->status  = 0x01fc; // FIXME
    regs->control = 1 << 6;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));

    // Start
    regs->ch1Range  = PWM_CHANNEL_RANGE;
    regs->dmaConfig = (1 << 31) | 0x707; //(7 << 8) | (7 << 0);
    regs->control   = (1 << 5)  | (1 << 2) | (1 << 0);
}

Pwm::~Pwm() 
{
    // Stop
    regs->control = 0x00000000;
}
