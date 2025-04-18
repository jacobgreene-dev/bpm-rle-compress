#include "rle_common.hh"
#include "rle_decompress.hh"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <array>

void decompressRLE(const std::string &inputRLE, const std::string &outputBMP)
{
    auto in = std::make_unique<std::ifstream>(inputRLE, std::ios::binary);
    if (!in->is_open())
    {
        std::cerr << "Error: Failed to open input RLE file: " << inputRLE << std::endl;
        return;
    }

    std::array<unsigned char, 54> header;
    in->read(reinterpret_cast<char *>(header.data()), header.size());
    if (in->gcount() != header.size())
    {
        std::cerr << "Error: RLE file does not contain a valid BMP header." << std::endl;
        return;
    }

    std::vector<RLEPixel> compressed;
    RLEPixel entry;
    while (in->read(reinterpret_cast<char *>(&entry), sizeof(RLEPixel)))
    {
        compressed.push_back(entry);
    }

    std::vector<unsigned char> pixels;
    for (const auto &p : compressed)
    {
        for (int i = 0; i < p.count; ++i)
        {
            pixels.push_back(p.blue);
            pixels.push_back(p.green);
            pixels.push_back(p.red);
        }
    }

    auto out = std::make_unique<std::ofstream>(outputBMP, std::ios::binary);
    if (!out->is_open())
    {
        std::cerr << "Error: Failed to create output BMP file: " << outputBMP << std::endl;
        return;
    }
    out->write(reinterpret_cast<char *>(header.data()), header.size());
    out->write(reinterpret_cast<char *>(pixels.data()), pixels.size());

    std::cout << "Decompression complete. Image written to: " << outputBMP << std::endl;
}
