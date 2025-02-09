#include "Peripherals.hpp"
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

Peripherals::Peripherals() 
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0)
    {
        throw std::runtime_error("Failed to open /dev/mem (Permission Denied)");
    }

    peripherals = mmap(
        nullptr,
        getSize(),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        getVirtualBaseAddress()
    );

    close(fd);

    if (peripherals == MAP_FAILED)
    {
        throw std::runtime_error("Unable to access Peripherals (mmap error)");
    }
}

Peripherals::~Peripherals() 
{
    munmap(peripherals, getSize());
}
