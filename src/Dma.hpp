#pragma once

#include "Device.hpp"

class Dma : public Device
{
public:
    Dma(std::uint32_t controlBlockAddress, std::uint32_t channel);
    ~Dma();

    inline void setcontrolBlockAddress(std::uint32_t address) 
    {
        regs->cbAddr = address;
    }

    inline volatile std::uint32_t &getcontrolBlockAddress() const 
    {
        return regs->cbAddr;
    }

    struct ControlBlock {
        std::uint32_t transferInfo; // Transfer Information
        std::uint32_t srcAddr;          // Source Address
        std::uint32_t destAddr;     // Destination Address
        std::uint32_t transferLen;  // Transfer Length (in bytes)
        std::uint32_t stride;       // 2D Stride (in bytes)
        std::uint32_t nextCb;         // Address of next Control Block
        std::uint32_t padding[2];     // Padding (To make 32-byte aligned)
    };

private:
    struct Regs {
        std::uint32_t control;         // Control and Status
        std::uint32_t cbAddr;         // Control Block Address
        std::uint32_t transferInfo; // Transfer Information
        std::uint32_t srcAddr;      // Source Address
        std::uint32_t destAddr;     // Destination Address
        std::uint32_t transferLen;  // Transfer Length (in bytes)
        std::uint32_t stride;         // 2D Stride (in bytes)
        std::uint32_t nextCb;         // Address of next Control Block
        std::uint32_t debug;         // Debug
    };

    volatile Regs *regs;
};
