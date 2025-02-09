#include "Memory.hpp"
#include "mailbox.h"

#include <string>
#include <stdexcept>

#define PAGE_SIZE 4096

Memory::Memory(std::size_t size) 
{
    // Make the size a multiple of PAGE_SIZE
    //size = ((size + PAGE_SIZE - 1)) / PAGE_SIZE * PAGE_SIZE; // FIXME??
    if (size % PAGE_SIZE)
        this->size = (size / PAGE_SIZE + 1) * PAGE_SIZE;
    else
        this->size = size;

    fd = mbox_open();
    handle = mem_alloc(fd, this->size, PAGE_SIZE, 0x0c);

    if (!handle) {
        mbox_close(fd);
        this->size = 0;
        
        throw std::runtime_error("Unable to allocate memory (" + std::to_string(size) + " bytes)");
    }

    busAddress = mem_lock(fd, handle);
    virtualAddress = mapmem(busAddress & ~0xc0000000, this->size);
}

Memory::~Memory() 
{
    unmapmem(virtualAddress, size);
    mem_unlock(fd, handle);
    mem_free(fd, handle);
    mbox_close(fd);
}
