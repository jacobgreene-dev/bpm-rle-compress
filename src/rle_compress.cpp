#include "rle_compress.hh"
#include "rle_common.hh"
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>

void compressBMP(const std::string &inputBMP, const std::string &outputRLE)
{
    auto file = std::make_unique<std::ifstream>(inputBMP, std::ios::binary);
    if (!file->is_open())
    {
        std::cerr << "Failed to open BMP file: " << inputBMP << std::endl;
        return;
    }

    std::array<unsigned char, 54> header;
    file->read(reinterpret_cast<char *>(header.data()), header.size());
    if (file->gcount() != header.size())
    {
        std::cerr << "Error: Failed to read BMP header from file: " << inputBMP << std::endl;
        return;
    }

    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    int row_padded = (width * 3 + 3) & (~3);

    std::cout << "Width: " << width << ", Height: " << height << std::endl;

    std::vector<unsigned char> data(row_padded * height);
    file->read(reinterpret_cast<char *>(data.data()), data.size());
    if (file->gcount() != static_cast<std::streamsize>(data.size()))
    {
        std::cerr << "Error: Failed to read the BMP pixel data from file: " << inputBMP << std::endl;
        return;
    }

    auto out = std::make_unique<std::ofstream>(outputRLE, std::ios::binary);
    if (!out->is_open())
    {
        std::cerr << "Error: Failed to create output RLE file: " << outputRLE << std::endl;
        return;
    }
    if (!out->write(reinterpret_cast<char *>(header.data()), header.size()))
    {
        std::cerr << "Error: Failed to write BMP header to output file: " << outputRLE << std::endl;
        return;
    }

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
        if (!out->write(reinterpret_cast<const char *>(&p), sizeof(RLEPixel)))
        {
            std::cerr << "Error: Failed to write compressed data to output file: " << outputRLE << std::endl;
            return;
        }
    }

    std::cout << "Compression complete. " << compressed.size() << " runs saved to " << outputRLE << std::endl;

    size_t origSize = header.size() + data.size();
    size_t compressedSize = header.size() + compressed.size() * sizeof(RLEPixel);
    double ratio = 100.0 * compressedSize / origSize;

    std::cout << "Original size:   " << origSize << " bytes" << std::endl;
    std::cout << "Compressed size: " << compressedSize << " bytes" << std::endl;
    std::cout << "Compression ratio: " << ratio << "%" << std::endl;
}
