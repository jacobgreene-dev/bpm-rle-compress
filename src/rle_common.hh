#ifndef RLE_COMMON_HH
#define RLE_COMMON_HH

#include <cstdint>

#pragma pack(push, 1)
struct RLEPixel
{
    uint8_t count;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
#pragma pack(pop)

#endif // RLE_COMMON_HH