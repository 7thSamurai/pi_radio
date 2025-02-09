#pragma once

#include "Peripherals.hpp"

class Device
{
public:
    Device() = default;
    ~Device() = default;

    static const std::uintptr_t DMA_BASE_OFFSET  = 0x00007000;
    static const std::uintptr_t CLK_BASE_OFFSET  = 0x00101070;
    static const std::uintptr_t GPIO_BASE_OFFSET = 0x00200000;
    static const std::uintptr_t PWM_BASE_OFFSET  = 0x0020c000;
};
