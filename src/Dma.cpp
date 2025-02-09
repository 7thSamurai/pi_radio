#include "Dma.hpp"
#include <cassert>

Dma::Dma(std::uint32_t controlBlockAddress, std::uint32_t channel) 
{
    assert(channel < 16);

    auto &peripherals = Peripherals::getInstance();
    regs = reinterpret_cast<Regs*>(peripherals.getVirtualAddress(Device::DMA_BASE_OFFSET + channel * 0x100));

    // Reset
    regs->control = 1 << 31;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));

    // Start
    regs->control = (1 << 2) | (1 << 1);
    regs->cbAddr  = controlBlockAddress;
    regs->control = (0xff << 16) | 0x1;
}

Dma::~Dma() 
{
    // Stop
    regs->control = 1 << 31;
}
