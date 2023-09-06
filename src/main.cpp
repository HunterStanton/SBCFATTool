#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include "FileTableEntry.h"

int main(int argc, char *argv[])
{

    if (argc != 5)
    {
        std::cout << "Usage: " << argv[0] << " unpack <filename.fat> <filename.dat> <out directory>" << std::endl;
        return 1;
    }

    if (std::string(argv[1]) == "unpack")
    {
        std::ifstream fatFile(argv[2], std::ios::binary);
        std::ifstream datFile(argv[3], std::ios::binary);

        if (!fatFile.is_open())
        {
            std::cout << "Error: could not open FAT file " << argv[2] << std::endl;
            return 1;
        }
        else if (!datFile.is_open())
        {
            std::cout << "Error: could not open DAT file " << argv[3] << std::endl;
            return 1;
        }

        // check for FAT magic
        // there is no DAT magic so can't check that
        char fatHeader[4];
        fatFile.read(fatHeader, 4);
        if (std::memcmp(fatHeader, "\x46\x41\x54\x20", 4) != 0)
        {
            std::cout << "Error: FAT file " << argv[2] << " is not a valid FAT file, invalid header" << std::endl;
            return 1;
        }

        int fileCount;
        fatFile.read(reinterpret_cast<char *>(&fileCount), sizeof(int));
        std::vector<FileTableEntry> fileTable(fileCount);

        // do some sanity checks on file count, it should never be 0
        if (fileCount == 0)
        {
            std::cout << "Error: FAT file " << argv[2] << " is not a valid FAT file, file count is 0" << std::endl;
            return 1;
        }
        else if (fileCount < 0)
        {
            std::cout << "Error: FAT file " << argv[2] << " is not a valid FAT file, file count is negative" << std::endl;
            return 1;
        }

        std::cout
            << "File count: " << fileCount << std::endl;

        // jump to 0xF4, the location of a pointer to the file table
        fatFile.seekg(0xF4);

        int fileTableOffset;
        fatFile.read(reinterpret_cast<char *>(&fileTableOffset), sizeof(int));

        // jump to the start of the file entry table
        fatFile.seekg(fileTableOffset);

        for (auto &entry : fileTable)
        {
            fatFile.read(reinterpret_cast<char *>(&entry.datOffset), sizeof(int));
            fatFile.read(reinterpret_cast<char *>(&entry.fileSize), sizeof(int));
            fatFile.read(reinterpret_cast<char *>(&entry.unk), sizeof(int));
            fatFile.read(reinterpret_cast<char *>(&entry.fileNameOffset), sizeof(int));

            std::streampos currPos = fatFile.tellg();
            fatFile.seekg(entry.fileNameOffset);

            std::getline(fatFile, entry.fileName, '\0');
            std::replace(entry.fileName.begin(), entry.fileName.end(), '\\', '/');
            fatFile.seekg(currPos);

            datFile.seekg(entry.datOffset);

            entry.data = std::make_unique<char[]>(entry.fileSize);
            datFile.read(entry.data.get(), entry.fileSize);

            std::filesystem::path outputFilePath = std::filesystem::path(argv[4]) / entry.fileName;
            std::filesystem::create_directories(outputFilePath.parent_path());

            std::cout << "Writing file " << entry.fileName << " to path " << outputFilePath << std::endl;

            std::ofstream outFile(outputFilePath, std::ios::binary);
            if (!outFile)
            {
                std::cout << "Error: could not create output file " << outputFilePath << std::endl;
                continue; // try the next file, maybe it will work
            }

            outFile.write(entry.data.get(), entry.fileSize);
        }
    }
    else
    {
        std::cout << "Error: unknown command " << argv[1] << std::endl;
        return 1;
    }

    return 0;
}