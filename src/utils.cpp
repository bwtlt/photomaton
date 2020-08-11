#include "utils.h"

#include <sys/time.h>
#include <cstdint>
#include <cstddef>

unsigned int getTimeInMillis (void)
{
    struct timeval tv;
    uint64_t now;

    gettimeofday(&tv, NULL);
    now = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);

    return (uint32_t)now;
}
