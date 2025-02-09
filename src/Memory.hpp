#pragma once

#include <cstdint>

class Memory
{
public:
    Memory(std::size_t size);
    ~Memory();

    inline std::uintptr_t getPhysicalAddress(volatile void *object) const 
    {
        return busAddress + (reinterpret_cast<std::uintptr_t>(object) - reinterpret_cast<std::uintptr_t>(virtualAddress));
    }

    inline std::uintptr_t getBaseAddress() const 
    {
        return reinterpret_cast<std::uintptr_t>(virtualAddress);
    }

private:
    void *virtualAddress;
    std::uintptr_t busAddress;
    std::uint32_t size;

    std::uint32_t handle;
    int fd; // Memory Box File Descriptor
};
