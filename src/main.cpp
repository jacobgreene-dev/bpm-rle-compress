#include "rle_compress.hh"
#include "rle_decompress.hh"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "No command provided. Use --help for usage." << std::endl;
        return EXIT_FAILURE;
    }

    std::string command = argv[1];

    if ((command == "--help" || command == "-h"))
    {
        std::cout << "Usage:\n"
                  << "  " << argv[0] << " <command> [options]\n\n"
                  << "Commands:\n"
                  << "  compress <input.bmp> <output.rleimg>\n"
                  << "      Compress a BMP file into an RLE image.\n\n"
                  << "  decompress <input.rleimg> <output.bmp>\n"
                  << "      Decompress an RLE image into a BMP file.\n\n"
                  << "Options:\n"
                  << "  --help, -h\n"
                  << "      Display this help message.\n\n"
                  << "Examples:\n"
                  << "  " << argv[0] << " compress ~\\static\\snail.bmp ~\\static\\output.rleimg\n"
                  << "  " << argv[0] << " decompress ~\\static\\output.rleimg ~\\static\\new.bmp\n";
        return EXIT_SUCCESS;
    }

    if (command == "compress" && argc == 4)
    {
        compressBMP(argv[2], argv[3]);
    }
    else if (command == "decompress" && argc == 4)
    {
        decompressRLE(argv[2], argv[3]);
    }
    else
    {
        std::cerr << "Invalid arguments. Use --help for usage." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}