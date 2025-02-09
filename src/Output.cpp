#include "Output.hpp"

Output::Output(std::uint32_t divisor) 
    : Clock(Device::CLK_BASE_OFFSET, divisor) 
{
    auto &peripherals = Peripherals::getInstance();
    output = reinterpret_cast<std::uint32_t*>(peripherals.getVirtualAddress(Device::GPIO_BASE_OFFSET));
    *output = (*output & 0xffff8fff) | (4 << 12); // TODO
}

Output::~Output() 
{
    *output = (*output & 0xffff8fff) | (1 << 12); // TODO
}
