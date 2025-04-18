#include "rle_common.hh"
#include "rle_decompress.hh"
#include <fstream>
#include <iostream>
#include <vector>

void decompressRLE(const std::string &inputRLE, const std::string &outputBMP)
{
    std::ifstream in(inputRLE, std::ios::binary);
    if (!in)
    {
        std::cerr << "Error: Failed to open input RLE file: " << inputRLE << std::endl;
        return;
    }

    unsigned char header[54];
    in.read(reinterpret_cast<char *>(header), 54);
    if (in.gcount() != 54)
    {
        std::cerr << "Error: RLE file does not contain a valid BMP header." << std::endl;
        return;
    }

    std::vector<RLEPixel> compressed;
    RLEPixel entry;
    while (in.read(reinterpret_cast<char *>(&entry), sizeof(RLEPixel)))
    {
        compressed.push_back(entry);
    }
    in.close();

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

    std::ofstream out(outputBMP, std::ios::binary);
    if (!out)
    {
        std::cerr << "Error: Failed to create output BMP file: " << outputBMP << std::endl;
        return;
    }
    out.write(reinterpret_cast<char *>(header), 54);
    out.write(reinterpret_cast<char *>(pixels.data()), pixels.size());
    out.close();

    std::cout << "Decompression complete. Image written to: " << outputBMP << std::endl;
}
