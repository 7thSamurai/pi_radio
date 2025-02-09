#pragma once

#include <cstdint>
#include <chrono>
#include <thread>
#include <bcm_host.h>

class Peripherals
{
public:
    Peripherals();
    ~Peripherals();

    // Singleton pattern
    static Peripherals &getInstance() 
    {
        static Peripherals instance;
        return instance;
    }

    inline std::uintptr_t getPhysicalAddress(volatile void *object) const 
    {
        return 0x7e000000 + (reinterpret_cast<uintptr_t>(object) - reinterpret_cast<uintptr_t>(peripherals));
    }

    inline std::uintptr_t getVirtualAddress(std::uintptr_t offset) const 
    {
        return reinterpret_cast<std::uintptr_t>(peripherals) + offset;
    }

    inline static std::uintptr_t getVirtualBaseAddress() 
    {
        return bcm_host_get_peripheral_address(); // FIXME
    }

    inline static float getClockFrequency() 
    {
        return 500.0f; // MGHz
    }

private:
    std::size_t getSize() const 
    {
        return bcm_host_get_peripheral_size();
    }

    void *peripherals;
};
