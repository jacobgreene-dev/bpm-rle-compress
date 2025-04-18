#include "rle_compress.hh"
#include "rle_common.hh"
#include <fstream>
#include <iostream>
#include <vector>

void compressBMP(const std::string &inputBMP, const std::string &outputRLE)
{
    std::ifstream file(inputBMP, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open BMP file: " << inputBMP << std::endl;
        return;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char *>(header), 54);

    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    int row_padded = (width * 3 + 3) & (~3);

    std::cout << "Width: " << width << ", Height: " << height << std::endl;

    std::vector<unsigned char> data(row_padded * height);
    file.read(reinterpret_cast<char *>(data.data()), data.size());
    file.close();

    std::ofstream out(outputRLE, std::ios::binary);
    if (!out)
    {
        std::cerr << "Error: Failed to create output RLE file: " << outputRLE << std::endl;
        return;
    }
    out.write(reinterpret_cast<char *>(header), 54);

    std::vector<RLEPixel> compressed;
    int idx = 0;
    int dataSize = data.size();

    while (idx + 3 <= dataSize)
    {
        unsigned char b = data[idx];
        unsigned char g = data[idx + 1];
        unsigned char r = data[idx + 2];

        uint8_t count = 1;
        while (idx + count * 3 + 3 <= dataSize && count < 255 &&
               data[idx + count * 3] == b &&
               data[idx + count * 3 + 1] == g &&
               data[idx + count * 3 + 2] == r)
        {
            ++count;
        }

        compressed.push_back({count, b, g, r});
        idx += count * 3;
    }

    for (const auto &p : compressed)
    {
        out.write(reinterpret_cast<const char *>(&p), sizeof(RLEPixel));
    }

    out.close();

    std::cout << "Compression complete. "
              << compressed.size()
              << " runs saved to "
              << outputRLE << std::endl;
}
